local netData = {}

local fn = __fn
local bitOps = gModules.bitOps
local tf = gModules.tf
local rtUtils = gModules.rtUtils

netData.Client = {}
netData.Client.OffsetCache = {
	dmap = {},
	smap = {},
}

netData.Server = {}
netData.Server.OffsetCache = {
	dmap = {},
	smap = {},
}

local inst = tf.mem.inst
local csts = tf.Consts

local hIClientB = inst.baseClient
local hIServerB = inst.baseServer
local hIServerE = inst.engineServer

netData.Server.g_pSharedChangeInfo = fn(function()
	if (hIServerE ~= nil) then
		return hIServerE.vmt.GetSharedEdictChangeInfo(hIServerE)
	end

	return nil
end
)

---@type fun(sendTable : ffi.cdata*, propStr : string):integer
netData.Server.tableGetNetVarOffset = rtUtils.memoize(netData.Server.OffsetCache.smap, 2,
	function(sendTable, propStr)
		local propNum = sendTable.m_nProps
		local prop = nil
		local offset = nil

		local dataTable = nil

		for i = 0, propNum - 1, 1 do
			prop = sendTable.m_pProps + i

			if (rtUtils.cLuaStrEql(prop.m_pVarName, propStr)) then
				return prop.m_Offset
			end

			dataTable = prop.m_pDataTable

			if (dataTable ~= nil) then
				offset = netData.Server.tableGetNetVarOffset(dataTable, propStr)

				if (offset) then
					return offset + prop.m_Offset
				end
			end
		end

		return nil
	end
)

netData.Server.classGetNetVarOffset = function(networkName, propStr)
	local currentEntry = hIServerB.vmt.GetAllServerClasses(hIServerB)

	repeat
		if (rtUtils.cLuaStrEql(currentEntry.m_pNetworkName, networkName)) then
			return netData.Server.tableGetNetVarOffset(currentEntry.m_pTable, propStr)
		end

		currentEntry = currentEntry.m_pNext
	until (currentEntry == nil)

	return nil
end

-- typedescription_t is broken or misaligned, or this is wrong. Do not use.
---@type fun(baseDMap : ffi.cdata*, propStr : string):integer
netData.Server.dataMapGetDataVarOffset = rtUtils.memoize(netData.Server.OffsetCache.dmap, 2,
	function(baseDMap, propStr)
		local dataMap = baseDMap
		local propDtMap = nil
		local offset = nil
		local desc = nil

		while (dataMap ~= nil) do
			for i = 0, dataMap.dataNumFields - 1, 1 do
				desc = dataMap.dataDesc + i

				if (rtUtils.cLuaStrEql(desc.fieldName, propStr)) then
					return desc.fieldOffset[ffi.C.TD_OFFSET_NORMAL]
				end

				propDtMap = desc.td

				if (propDtMap ~= nil) then
					offset = netData.Server.dataMapGetDataVarOffset(propDtMap, propStr)

					if (offset) then
						return offset
					end
				end
			end

			dataMap = dataMap.baseMap
		end

		return nil
	end
)

-- typedescription_t is broken or misaligned. Do not use.
netData.Server.entGetDataVarOffset = function(cBase, propStr)
	return netData.Server.dataMapGetDataVarOffset(cBase.vmt.GetDataDescMap(cBase), propStr)
end

netData.Server.edictStateChangeOffset = function(edict, offset)
	local sharedCI = netData.Server.g_pSharedChangeInfo

	if (sharedCI == nil) then
		return
	end

	local bEdict = edict.m_CBaseEdict

	if (bitOps.band(bEdict.m_fStateFlags, csts.FL_FULL_EDICT_CHANGED) ~= 0) then
		return
	end

	bEdict.m_fStateFlags = bitOps.bor(bEdict.m_fStateFlags, csts.FL_EDICT_CHANGED)

	local accessor = hIServerE.vmt.GetChangeAccessor(hIServerE, edict)

	if (accessor.m_iChangeInfoSerialNumber == sharedCI.m_iSerialNumber) then
		local p = sharedCI.m_ChangeInfos + accessor.m_iChangeInfo

		for i = 0, p.m_nChangeOffsets - 1, 1 do
			if (p.m_ChangeOffsets[i] == offset) then
				return
			end
		end

		if (p.m_nChangeOffsets == csts.MAX_CHANGE_OFFSETS) then
			accessor.m_iChangeInfoSerialNumber = 0
			bEdict.m_fStateFlags = bitOps.bor(bEdict.m_fStateFlags, csts.FL_FULL_EDICT_CHANGED)
		else
			p.m_ChangeOffsets[p.m_nChangeOffsets] = offset
			p.m_nChangeOffsets = p.m_nChangeOffsets + 1
		end
	else
		if (sharedCI.m_nChangeInfos == csts.MAX_EDICT_CHANGE_INFOS) then
			accessor.m_iChangeInfoSerialNumber = 0
			bEdict.m_fStateFlags = bitOps.bor(bEdict.m_fStateFlags, csts.FL_FULL_EDICT_CHANGED)
		else
			accessor.m_iChangeInfo = sharedCI.m_nChangeInfos
			sharedCI.m_nChangeInfos = sharedCI.m_nChangeInfos + 1

			accessor.m_iChangeInfoSerialNumber = sharedCI.m_iSerialNumber

			local p = sharedCI.m_ChangeInfos + accessor.m_iChangeInfo
			p.m_ChangeOffsets[0] = offset
			p.m_nChangeOffsets = 1
		end
	end
end

netData.Server.getEntNetProp = function(edict, propStr, reflType)
	if (edict == nil) then
		return nil
	end

	local eUnkn = edict.m_CBaseEdict.m_pUnk
	local eNtwk = edict.m_CBaseEdict.m_pNetworkable

	local serverClass = eNtwk.vmt.GetServerClass(eNtwk)
	local offset = netData.Server.classGetNetVarOffset(ffi.string(serverClass.m_pNetworkName), propStr)

	if (not offset) then
		return nil
	end

	local cast = ffi.cast(reflType .. "*", ffi.cast("uintptr_t", eUnkn.vmt.GetBaseEntity(eUnkn)) + offset)

	return cast
end

netData.Server.setEntNetProp = function(edict, propStr, reflType, val, forceChange)
	if (edict == nil) then
		return nil
	end

	if (forceChange == nil) then
		forceChange = true
	end

	local eUnkn = edict.m_CBaseEdict.m_pUnk
	local eNtwk = edict.m_CBaseEdict.m_pNetworkable

	local serverClass = eNtwk.vmt.GetServerClass(eNtwk)
	local offset = netData.Server.classGetNetVarOffset(ffi.string(serverClass.m_pNetworkName), propStr)

	if (not offset) then
		return nil
	end

	local cast = ffi.cast(reflType .. "*", ffi.cast("uintptr_t", eUnkn.vmt.GetBaseEntity(eUnkn)) + offset)
	cast[0] = ffi.cast(reflType, val)

	if (forceChange) then
		netData.Server.edictStateChangeOffset(edict, offset)
	end

	return true
end

-- typedescription_t is broken or misaligned. Do not use.
netData.Server.getEntDataProp = function(edict, propStr, reflType)
	if (edict == nil) then
		return nil
	end

	local eUnkn = edict.m_CBaseEdict.m_pUnk
	local cBase = eUnkn.vmt.GetBaseEntity(eUnkn)

	local offset = netData.Server.entGetDataVarOffset(cBase, propStr)

	if (not offset) then
		return nil
	end

	local cast = ffi.cast(reflType .. "*", ffi.cast("uintptr_t", cBase) + offset)

	return cast
end

-- typedescription_t is broken or misaligned. Do not use.
netData.Server.setEntDataProp = function(edict, propStr, reflType, val, forceChange)
	if (edict == nil) then
		return nil
	end

	if (forceChange == nil) then
		forceChange = true
	end

	local eUnkn = edict.m_CBaseEdict.m_pUnk
	local cBase = eUnkn.vmt.GetBaseEntity(eUnkn)

	local offset = netData.Server.entGetDataVarOffset(cBase, propStr)

	if (not offset) then
		return nil
	end

	local cast = ffi.cast(reflType .. "*", ffi.cast("uintptr_t", cBase) + offset)
	cast[0] = ffi.cast(reflType, val)

	if (forceChange) then
		netData.Server.edictStateChangeOffset(edict, offset)
	end

	return true
end

---@type fun(recvTable : ffi.cdata*, propStr : string):integer
netData.Client.tableGetNetVarOffset = rtUtils.memoize(netData.Client.OffsetCache.smap, 2,
	function(recvTable, propStr)
		local propNum = recvTable.m_nProps
		local prop = nil
		local offset = nil

		local dataTable = nil

		for i = 0, propNum - 1, 1 do
			prop = recvTable.m_pProps + i

			if (rtUtils.cLuaStrEql(prop.m_pVarName, propStr)) then
				return prop.m_Offset
			end

			dataTable = prop.m_pDataTable

			if (dataTable ~= nil) then
				offset = netData.Client.tableGetNetVarOffset(dataTable, propStr)

				if (offset) then
					return offset + prop.m_Offset
				end
			end
		end

		return nil
	end
)

netData.Client.classGetNetVarOffset = function(networkName, propStr)
	local currentEntry = hIClientB.vmt.GetAllClasses(hIClientB)

	repeat
		if (rtUtils.cLuaStrEql(currentEntry.m_pNetworkName, networkName)) then
			return netData.Client.tableGetNetVarOffset(currentEntry.m_pRecvTable, propStr)
		end

		currentEntry = currentEntry.m_pNext
	until (currentEntry == nil)

	return nil
end

netData.Client.tableGetNetVar = function(recvTable, propStr)
	local propNum = recvTable.m_nProps
	local prop = nil

	local dataTable = nil

	for i = 0, propNum - 1, 1 do
		prop = recvTable.m_pProps + i

		if (rtUtils.cLuaStrEql(prop.m_pVarName, propStr)) then
			return prop
		end

		dataTable = prop.m_pDataTable

		if (dataTable ~= nil) then
			prop = netData.Client.tableGetNetVar(dataTable, propStr)

			if (prop) then
				return prop
			end
		end
	end

	return nil
end

netData.Client.classGetNetVar = function(networkName, propStr)
	local currentEntry = hIClientB.vmt.GetAllClasses(hIClientB)

	repeat
		if (rtUtils.cLuaStrEql(currentEntry.m_pNetworkName, networkName)) then
			return netData.Client.tableGetNetVar(currentEntry.m_pRecvTable, propStr)
		end

		currentEntry = currentEntry.m_pNext
	until (currentEntry == nil)

	return nil
end

return netData
