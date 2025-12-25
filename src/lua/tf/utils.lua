local utils = {}

utils.Client = {}
utils.Server = {}

local bitOps = gModules.bitOps
local tf = gModules.tf

local inst = tf.mem.inst
local csts = tf.Consts

local hIServerE = inst.engineServer

utils.ConColors = {
	red = ffi.new("Color_t", { 255, 0, 0, 255 }),
	green = ffi.new("Color_t", { 0, 255, 0, 255 }),
	blue = ffi.new("Color_t", { 0, 0, 255, 255 }),
	yellow = ffi.new("Color_t", { 255, 255, 0, 255 }),
	white = ffi.new("Color_t", { 255, 255, 255, 255 }),
}

utils.Server.lookupEntity = function(index)
	if ((index < 0) or (index >= csts.MAX_EDICTS)) then
		return nil
	end

	return hIServerE.vmt.PEntityOfEntIndex(hIServerE, index)
end

utils.Server.referenceToEdict = function(ref)
	if (bitOps.band(ref, csts.ENTREF_MASK) ~= 0) then
		return utils.Server.lookupEntity(bitOps.band(ref, bitOps.bnot(csts.ENTREF_MASK)))
	end

	return utils.Server.lookupEntity(ref)
end

utils.Server.edictToReference = function(edict)
	local eUnkn = edict.m_CBaseEdict.m_pUnk

	return bitOps.bor(eUnkn.vmt.vmt_IHandleEntity.GetRefEHandle(eUnkn).m_Index, csts.ENTREF_MASK)
end

return utils
