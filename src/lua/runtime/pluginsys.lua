local pluginSys = {}

local rtUtils = gModules.rtUtils

pluginSys.Types = {}

---@class psT_FuncHook
pluginSys.Types.FuncHook = {
	---@type any
	hookCtx = nil,
	cbs = nil,

	new = __MakeClass,

	---@param class psT_FuncHook
	---@param bTable psT_FuncHook
	init = function(class, bTable, orig, hook)
		__MakeClass(class, bTable)

		bTable.hookCtx = ffi.new("detour_ctx_t")

		CExt.ldtour.detour_init(bTable.hookCtx, orig, hook)

		bTable.cbs = {}

		return bTable
	end,

	---@param self psT_FuncHook
	replace = function(self)
		CExt.ldtour.detour_enable(self.hookCtx)
	end,

	---@param self psT_FuncHook
	restore = function(self)
		CExt.ldtour.detour_disable(self.hookCtx)
	end,

	---@param self psT_FuncHook
	addCB = function(self, cb)
		table.insert(self.cbs, cb)
	end,

	---@param self psT_FuncHook
	removeCB = function(self, cb)
		for index, func in ipairs(self.cbs) do
			if (func == cb) then
				table.remove(self.cbs, index)
			end
		end
	end,
}

---@class psT_VMTHook : psT_FuncHook
pluginSys.Types.VMTHook = pluginSys.Types.FuncHook:new({})
pluginSys.Types.VMTHook.orig = nil
---@type integer
pluginSys.Types.VMTHook.origIndex = nil
pluginSys.Types.VMTHook.hookFunc = nil

---@param class psT_VMTHook
---@param bTable psT_VMTHook
---@diagnostic disable-next-line
pluginSys.Types.VMTHook.init = function(class, bTable, orig, origIndex, vmt, hookFunc)
	__MakeClass(class, bTable)

	bTable.hookCtx = ffi.new("lm_vmt_t")

	if (CExt.lmem.LM_VmtNew(ffi.cast("void*", vmt), bTable.hookCtx) == ffi.C.LM_TRUE) then
		bTable.hookCtx = ffi.gc(bTable.hookCtx, CExt.lmem.LM_VmtFree)
	else
		bTable.hookCtx = nil
	end

	bTable.orig = orig
	bTable.origIndex = origIndex
	bTable.hookFunc = hookFunc
	bTable.cbs = {}

	return bTable
end

---@param self psT_VMTHook
---@diagnostic disable-next-line
pluginSys.Types.VMTHook.replace = function(self)
	CExt.lmem.LM_VmtHook(self.hookCtx, self.origIndex, ffi.cast("uintptr_t", self.hookFunc))
end

---@param self psT_VMTHook
---@diagnostic disable-next-line
pluginSys.Types.VMTHook.restore = function(self)
	CExt.lmem.LM_VmtUnhook(self.hookCtx, self.origIndex)
end

---@class ps_PluginsManager
pluginSys.PluginsManager = {
	---@type psT_Plugin[]
	plugins = nil,
	---@type psT_FuncHook[]
	fHooks = nil,
	---@type psT_VMTHook[]
	vHooks = nil,

	new = __MakeClass,

	---@param class ps_PluginsManager
	---@param bTable ps_PluginsManager
	init = function(class, bTable)
		__MakeClass(class, bTable)

		bTable.plugins = {}
		bTable.fHooks = {}
		bTable.vHooks = {}

		return bTable
	end,

	---@param self ps_PluginsManager
	---@param func function
	checkAdd = function(self, member, pluginName, uID, func, ...)
		if (not self[member][pluginName]) then
			self[member][pluginName] = {}
		end

		local l1 = self[member][pluginName]

		if (not l1[uID]) then
			l1[uID] = {}
		end

		local ret = func(l1[uID], ...)

		if (ret ~= nil) then
			return ret
		end
	end,

	---@param self ps_PluginsManager
	---@param func function
	checkRemove = function(self, member, pluginName, uID, func, ...)
		local l1 = self[member][pluginName]

		if (not l1) then
			return
		end

		local l2 = l1[uID]

		if (not l2) then
			return
		end

		local ret = func(l2, ...)

		if (rtUtils.tableLen(l2) == 0) then
			l1[uID] = nil
		end

		if (ret ~= nil) then
			return ret
		end
	end,

	---@param self ps_PluginsManager
	---@param plugin psT_Plugin
	addPlugin = function(self, plugin)
		if (not plugin) then
			return
		end

		local name = plugin.name

		if (self.plugins[name]) then
			return
		end

		self.plugins[name] = plugin
	end,

	---@param self ps_PluginsManager
	---@param plugin psT_Plugin
	---@overload fun(self, plugin):psT_Plugin | nil
	getOrAddPlugin = function(self, plugin)
		if (not plugin) then
			return
		end

		local ret = nil

		ret = self.plugins[plugin.name]

		if (not ret) then
			self:addPlugin(plugin)
		end

		return ret
	end,

	---@param self ps_PluginsManager
	---@param plugin psT_Plugin
	reloadPlugin = function(self, plugin)
		self:removePlugin(plugin)
		self:addPlugin(plugin)
	end,

	---@param self ps_PluginsManager
	---@param plugin psT_Plugin
	removePlugin = function(self, plugin)
		if (not plugin) then
			return
		end

		if (plugin.deinitFunc) then
			plugin:deinitFunc()
		end

		local name = plugin.name

		self.fHooks[name] = nil
		self.vHooks[name] = nil
		self.plugins[name] = nil
	end,

	---@param self ps_PluginsManager
	updateScripts = function(self, ...)
		for _, value in pairs(self.plugins) do
			value:update(...)
		end
	end,
}

---@class psT_Plugin : utils_Script
pluginSys.Types.Plugin = rtUtils.Types.Script:new({})
---@type any
-- Use it for anything
pluginSys.Types.Plugin.__custom = nil
---@type fun(self : psT_Plugin)
pluginSys.Types.Plugin.deinitFunc = nil

-- Redesign these, maybe checkAdd/Remove as well.

---@param self psT_Plugin
---@param plman ps_PluginsManager
---@param fName string
---@param fHook psT_FuncHook
---@diagnostic disable-next-line
pluginSys.Types.Plugin.registerFHook = function(self, plman, fName, fHook)
	if (not plman.fHooks[self.name]) then
		plman.fHooks[self.name] = {}
	end

	local hooksList = plman.fHooks[self.name]

	if (hooksList[fName]) then
		return
	end

	hooksList[fName] = fHook
end

---@param self psT_Plugin
---@param plman ps_PluginsManager
---@param fName string
---@diagnostic disable-next-line
pluginSys.Types.Plugin.addFHookCb = function(self, plman, fName, cb)
	local hooksList = plman.fHooks[self.name]

	if (not hooksList) then
		return
	end

	---@type psT_FuncHook
	local fHook = hooksList[fName]

	if (not fHook) then
		return
	end

	fHook:addCB(cb)
end

---@param self psT_Plugin
---@param plman ps_PluginsManager
---@param fName string
---@diagnostic disable-next-line
pluginSys.Types.Plugin.removeFHookCb = function(self, plman, fName, cb)
	local hooksList = plman.fHooks[self.name]

	if (not hooksList) then
		return
	end

	---@type psT_FuncHook
	local fHook = hooksList[fName]

	if (not fHook) then
		return
	end

	fHook:removeCB(cb)
end

---@param self psT_Plugin
---@param plman ps_PluginsManager
---@param vName string
---@param vHook psT_VMTHook
---@diagnostic disable-next-line
pluginSys.Types.Plugin.registerVHook = function(self, plman, vName, vHook)
	if (not plman.vHooks[self.name]) then
		plman.vHooks[self.name] = {}
	end

	local hooksList = plman.vHooks[self.name]

	if (hooksList[vName]) then
		return
	end

	hooksList[vHook] = vHook
end

---@param self psT_Plugin
---@param plman ps_PluginsManager
---@param vName string
---@diagnostic disable-next-line
pluginSys.Types.Plugin.addVHookCb = function(self, plman, vName, cb)
	local hooksList = plman.vHooks[self.name]

	if (not hooksList) then
		return
	end

	---@type psT_VMTHook
	local vHook = hooksList[vName]

	if (not vHook) then
		return
	end

	vHook:addCB(cb)
end

---@param self psT_Plugin
---@param plman ps_PluginsManager
---@param vName string
---@diagnostic disable-next-line
pluginSys.Types.Plugin.removeVHookCb = function(self, plman, vName, cb)
	local hooksList = plman.vHooks[self.name]

	if (not hooksList) then
		return
	end

	---@type psT_VMTHook
	local vHook = hooksList[vName]

	if (not vHook) then
		return
	end

	vHook:removeCB(cb)
end

return pluginSys
