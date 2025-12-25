local tf = {}

local fn = __fn
local lcpp = gModules.lcpp
local bitOps = gModules.bitOps
local rtUtils = gModules.rtUtils
local pluginSys = gModules.pluginSys
local asy = gModules.asy

tf.Consts = {}

fn(function()
	-- The table is replaced on each use of cdef
	lcpp.cParser = lcpp.init("", ffi.lcpp_defs)

	local res, expr, valType, newParser = nil, nil, nil, nil

	for key, value in pairs(ffi.lcpp_defs) do
		valType = type(value)

		if (valType == "string") then
			res, expr = pcall(lcpp.cParser.parseExpr, lcpp.cParser, value)

			if (not res) then
				value = value:gsub("^%((.*)%)$", "%1")
				value = value:gsub("(%d*)(%.f)", "%1.0")
				value = value:gsub("(%d*)(f)", "%1")

				newParser = lcpp.init("", tf.Consts)
				res, expr = pcall(newParser.parseExpr, newParser, value)

				if (res) then
					tf.Consts[key] = expr
				end
			else
				tf.Consts[key] = expr
			end
		elseif ((valType == "number") or (valType == "boolean")) then
			tf.Consts[key] = value
		end
	end

	collectgarbage()
end)

tf.Types = {}

tf.Types.IntendedUseFlags = {
	Any = 0,
	Client = bitOps.lshift(1, 0),
	Server = bitOps.lshift(1, 1),
	VSP = bitOps.lshift(1, 2),
}

---@param class tfMT_Class | tfMT_Interface
tf.createClassPtr = function(class, cModule)
	local sig = class.signature

	if ((not sig) or (not cModule)) then
		return
	end

	local ciFn = CExt.lmem.LM_FindSymbolAddress(cModule, "CreateInterface")

	if (ciFn == nil) then
		print("Could not find \"CreateInterface\" in " .. class.cModule .. __DYN_LIB_SUF)

		return
	end

	local castCiFn = ffi.cast(
		"CreateInterfaceFn",
		ciFn
	)

	class.instance = castCiFn(sig, nil)

	if (class.instance == nil) then
		print("Could not create interface for " .. class.typeStr .. " | Sig : " .. sig)

		return
	end
end

---@param class tfMT_Class | tfMT_Interface
tf.getClassPtr = function(class, cModule)
	local sig = class.signature

	if ((not sig) or (not cModule)) then
		return
	end

	local sym = CExt.lmem.LM_FindSymbolAddress(cModule, sig)

	if (sym == nil) then
		print("Could not get interface symbol for " .. class.typeStr .. " | Sig : " .. sig)

		return
	end

	class.instance = ffi.cast(class.cType, sym)
end

---@param class tfMT_Class | tfMT_Interface
tf.scanClassPtr = function(class, cModule, offset)
	local sig = class.signature

	if ((not sig) or (not cModule)) then
		return
	end

	local sym = CExt.lmem.LM_SigScan(sig, cModule.base, cModule.size)

	if (sym == nil) then
		print("Could not find interface symbol for " .. class.typeStr .. " | Sig : " .. sig)

		return
	end

	class.instance = ffi.cast(class.cType, sym + (offset or 0))
end

---@param class tfMT_Class | tfMT_Interface
---@param bTable tfMT_Class | tfMT_Interface
tf.initClass = function(class, bTable, reflType)
	__MakeClass(class, bTable)

	bTable.typeStr = reflType
	bTable.cType = ffi.typeof(reflType .. "*")

	bTable.signature = tf.Consts[reflType .. "_SIGNATURE"]
	bTable.cModule = tf.Consts[reflType .. "_MODULE"]

	return bTable
end

tf.MemTypes = {}

---@class tfMT_Class
tf.MemTypes.Class = {
	---@type any
	instance = nil,
	---@type string
	typeStr = nil,
	cType = nil,
	---@type string
	signature = nil,
	---@type string
	cModule = nil,

	new = __MakeClass,

	getInstance = tf.createClassPtr,

	init = tf.initClass,
}

---@class tfMT_SymClass : tfMT_Class
tf.MemTypes.SymClass = tf.MemTypes.Class:new({})
tf.MemTypes.SymClass.getInstance = tf.getClassPtr

---@class tfMT_SigClass : tfMT_Class
tf.MemTypes.SigClass = tf.MemTypes.Class:new({})
tf.MemTypes.SigClass.getInstance = tf.scanClassPtr

---@class tfMT_Interface : tfMT_Class
tf.MemTypes.Interface = tf.MemTypes.Class:new({})

---@class tfMT_SymInterface : tfMT_Interface
tf.MemTypes.SymInterface = tf.MemTypes.Interface:new({})
tf.MemTypes.SymInterface.getInstance = tf.getClassPtr

---@class tfMT_SigInterface : tfMT_Interface
tf.MemTypes.SigInterface = tf.MemTypes.Interface:new({})
tf.MemTypes.SigInterface.getInstance = tf.scanClassPtr

tf.mem = {}

-- Must match the file name
---@type (ffi.cdata* | number)[]
tf.mem.libs = {
	client = 0,
	server = 0,
	engine = 0,
	vguimatsurface = 0,
	vgui2 = 0,
	materialsystem = 0,
	studiorender = 0,
	filesystem_stdio = 0,
	inputsystem = 0,
	tier0 = 0,
	vphysics = 0,
}

tf.mem.libs[tf.Consts["VSTDLIB_DEFINE"]] = 0

tf.mem.intfs = {
	engineClient = tf.MemTypes.Interface:init({}, "IVEngineClient"),
	engineServer = tf.MemTypes.Interface:init({}, "IVEngineServer"),
	baseClient = tf.MemTypes.Interface:init({}, "IBaseClientDLL"),
	baseServer = tf.MemTypes.Interface:init({}, "IBaseServerDLL"),
	clientEntityList = tf.MemTypes.Interface:init({}, "IClientEntityList"),
	engineVGui = tf.MemTypes.Interface:init({}, "IEngineVGui"),
	cvar = tf.MemTypes.Interface:init({}, "ICvar"),
	matSysSurface = tf.MemTypes.Interface:init({}, "IMatSystemSurface"),
	panel = tf.MemTypes.Interface:init({}, "IPanel"),
	modelInfoClient = tf.MemTypes.Interface:init({}, "IVModelInfoClient"),
	materialSystem = tf.MemTypes.Interface:init({}, "IMaterialSystem"),
	renderView = tf.MemTypes.Interface:init({}, "IVRenderView"),
	engineTrace = tf.MemTypes.Interface:init({}, "IEngineTrace"),
	modelRender = tf.MemTypes.Interface:init({}, "IVModelRender"),
	studioRender = tf.MemTypes.Interface:init({}, "IStudioRender"),
	gameMovement = tf.MemTypes.Interface:init({}, "CTFGameMovement"),
	prediction = tf.MemTypes.Interface:init({}, "CPrediction"),
	fileSystem = tf.MemTypes.Interface:init({}, "IFileSystem"),
	gameEventManager = tf.MemTypes.Interface:init({}, "IGameEventManager2"),
	inputSystem = tf.MemTypes.Interface:init({}, "IInputSystem"),
	memAlloc = tf.MemTypes.SymInterface:init({}, "IMemAlloc"),
	vphysics = tf.MemTypes.Interface:init({}, "IPhysics"),
	vphysicsCollision = tf.MemTypes.Interface:init({}, "IPhysicsCollision"),
	debugOverlay = tf.MemTypes.Interface:init({}, "IVDebugOverlay"),
	serverPlugin = tf.MemTypes.Interface:init({}, "IServerPluginCallbacks"),
	serverPluginHelpers = tf.MemTypes.Interface:init({}, "IServerPluginHelpers"),
	globalVarsBase = tf.MemTypes.SigClass:init({}, "CGlobalVarsBase"),
}

-- Expose code globally
tf.mem.gFuncs = {}
-- Expose code globally
tf.mem.gClasses = {}

tf.mem.inst = {}

tf.initInst = function()
	for key, value in pairs(tf.mem.intfs) do
		tf.mem.inst[key] = ffi.cast(value.cType, value.instance)
	end
end

---@class tf_PluginsManager : ps_PluginsManager
tf.PluginsManager = pluginSys.PluginsManager:new({})

---@type psT_FuncHook
tf.PluginsManager.serverEvHook = nil
---@type psT_FuncHook
tf.PluginsManager.clientEvHook = nil
tf.PluginsManager.eventsList = nil
tf.PluginsManager.clientEventsList = nil

---@param class tf_PluginsManager
---@param bTable tf_PluginsManager
---@diagnostic disable-next-line
tf.PluginsManager.init = function(class, bTable)
	__MakeClass(class, bTable)

	bTable.plugins = {}
	bTable.fHooks = {}
	bTable.vHooks = {}
	bTable.eventsList = {}
	bTable.clientEventsList = {}

	return bTable
end

---@diagnostic disable-next-line
tf.PluginsManager.initHooks = function(self, svEvCb, clEvCb)
	local gEvManVMT = tf.mem.inst.gameEventManager.vmt

	if (gEvManVMT == nil) then
		return
	end

	self.serverEvHook = pluginSys.Types.FuncHook:init(
		{},
		gEvManVMT.FireEvent,
		svEvCb
	)
	self.clientEvHook = pluginSys.Types.FuncHook:init(
		{},
		gEvManVMT.FireEventClientSide,
		clEvCb
	)
end

---@param plugin tfT_Plugin
---@diagnostic disable-next-line
tf.PluginsManager.removePlugin = function(self, plugin)
	if (not plugin) then
		return
	end

	if (plugin.deinitFunc) then
		plugin:deinitFunc()
	end

	local name = plugin.name

	self.fHooks[name] = nil
	self.vHooks[name] = nil
	self.eventsList[name] = nil
	self.clientEventsList[name] = nil
	self.plugins[name] = nil
end

---@class tfT_Plugin : psT_Plugin
tf.Types.Plugin = pluginSys.Types.Plugin:new({})

---@overload fun(class, bTable, path, name):tfT_Plugin
tf.Types.Plugin.init = pluginSys.Types.Plugin.init

---@param self tfT_Plugin
---@param plman tf_PluginsManager
---@param eventName string
---@diagnostic disable-next-line
tf.Types.Plugin.hookEvent = function(self, plman, eventName, clientMode, cb)
	local member = "eventsList"

	if (clientMode == true) then
		member = "clientEventsList"
	end

	return plman:checkAdd(member, self.name, eventName, table.insert, cb)
end

---@param self tfT_Plugin
---@param plman tf_PluginsManager
---@param eventName string
---@diagnostic disable-next-line
tf.Types.Plugin.unhookEvent = function(self, plman, eventName, clientMode, cb)
	local member = "eventsList"

	if (clientMode == true) then
		member = "clientEventsList"
	end

	return plman:checkRemove(member, self.name, eventName, function(funcsList, elem)
		for index, value in ipairs(funcsList) do
			if (value == elem) then
				table.remove(funcsList, index)
			end
		end
	end, cb)
end

return tf

-----@class tfT_ClVSPEventLoop : asy_EventLoop
---- The Steam Runtime has issues with some functions used by `asy.EventLoop`,
---- leading to crashes.
----
---- Use this only if you run the VSP on a TF2 Linux client.
----
---- Microseconds are the lowest time division.
----
---- NOTE : This may not be an issue anymore.
----
---- It seems to have been fixed a day later after a few crashes.
--tf.Types.ClVSPEventLoop = asy.EventLoop:new({})
--
-----@param class tfT_ClVSPEventLoop
-----@param bTable tfT_ClVSPEventLoop
-----@diagnostic disable-next-line
--tf.Types.ClVSPEventLoop.init = function(class, bTable)
--	__MakeClass(class, bTable)
--
--	bTable.funcs = {}
--
--	return bTable
--end
--
-----@param self tfT_ClVSPEventLoop
-----@param ... any
-----@diagnostic disable-next-line
--tf.Types.ClVSPEventLoop.timeout = function(self, func, delay, ...)
--	local timeoutFn = asy.Timeout:new({})
--
--	timeoutFn.func = func
--	timeoutFn.delay = delay
--	timeoutFn.timestamp = CExt.self.cexp_timeMicroTimestamp()
--
--	---@type table
--	local argsTable = { ... }
--	argsTable.__n = #argsTable
--	timeoutFn.args = argsTable
--
--	table.insert(self.funcs, timeoutFn)
--
--	return timeoutFn
--end
--
-----@param self tfT_ClVSPEventLoop
-----@param ... any
-----@diagnostic disable-next-line
--tf.Types.ClVSPEventLoop.periodic = function(self, func, delay, ...)
--	local periodicFn = asy.Periodic:new({})
--
--	periodicFn.func = func
--	periodicFn.delay = delay
--	periodicFn.timestamp = CExt.self.cexp_timeMicroTimestamp()
--
--	---@type table
--	local argsTable = { ... }
--	argsTable.__n = #argsTable
--	periodicFn.args = argsTable
--
--	table.insert(self.funcs, periodicFn)
--
--	return periodicFn
--end
--
-----@param self tfT_ClVSPEventLoop
-----@diagnostic disable-next-line
--tf.Types.ClVSPEventLoop.stop = function(self)
--	self.terminate = true
--end
--
-----@diagnostic disable-next-line
--local __unpack = table.unpack or unpack
--
-----@param self tfT_ClVSPEventLoop
-----@diagnostic disable-next-line
--tf.Types.ClVSPEventLoop.start = function(self)
--	self.started = true
--
--	local targetTime
--	local currentDelay
--
--	while true do
--		if (self.terminate) then
--			break
--		end
--
--		self.lowestDelay = nil
--		local now = CExt.self.cexp_timeMicroTimestamp()
--
--		for index, value in ipairs(self.funcs) do
--			targetTime = value.delay + value.timestamp
--
--			if (now >= targetTime) then
--				value.func(__unpack(value.args, 1, value.args.__n))
--
--				if (value.__classtype == asy.FuncClassTypes.Timeout) then
--					table.remove(self.funcs, index)
--				elseif (value.__classtype == asy.FuncClassTypes.Periodic) then
--					value.timestamp = now
--
--					if ((self.lowestDelay == nil) or (value.delay < self.lowestDelay)) then
--						self.lowestDelay = value.delay
--					end
--				end
--			else
--				currentDelay = targetTime - now
--
--				if ((self.lowestDelay == nil) or (currentDelay < self.lowestDelay)) then
--					self.lowestDelay = currentDelay
--				end
--			end
--		end
--
--		if ((self.lowestDelay ~= nil) and (self.lowestDelay > 0)) then
--			CExt.self.cexp_zigsleep(self.lowestDelay)
--		else
--			CExt.self.cexp_zigsleep(100 * asy.ns_per_us)
--		end
--	end
--end

---- If this is not first, the FireEvent hook below will crash.
--local __FireEventHookFix = ffi.cast(svEvHook, function(ptr, event, dontBroadcast)
--
--end
--)
--
--local FireEventHook = ffi.cast(svEvHook, function(ptr, event, dontBroadcast)
--	plman.serverEvHook:restore()
--
--	local eventName = ffi.string(event.vmt.GetName(event))
--	local refDontBroadcast = ffi.new("bool[1]", dontBroadcast)
--	local funcsList = nil
--	local ret = nil
--
--	for _, pluginEvents in pairs(plman.eventsList) do
--		funcsList = pluginEvents[eventName]
--
--		if (funcsList) then
--			for _, cb in ipairs(funcsList) do
--				ret = cb(ptr, event, refDontBroadcast)
--
--				if ((ret ~= nil) and (ret >= ffi.C.PLUGIN_STOP)) then
--					return false
--				end
--			end
--		end
--	end
--
--	local cast = ffi.cast(svEvHook, plman.serverEvHook.hookCtx.orig)
--
--	ret = cast(ptr, event, refDontBroadcast[0])
--
--	plman.serverEvHook:replace()
--
--	return ret
--end
--)
--
--local FireEventClientHook = ffi.cast(clEvHook, function(ptr, event)
--	plman.clientEvHook:restore()
--
--	local eventName = ffi.string(event.vmt.GetName(event))
--	local funcsList = nil
--	local ret = nil
--
--	for _, pluginEvents in pairs(plman.clientEventsList) do
--		funcsList = pluginEvents[eventName]
--
--		if (funcsList) then
--			for _, cb in ipairs(funcsList) do
--				ret = cb(ptr, event)
--
--				if (ret >= ffi.C.PLUGIN_STOP) then
--					return false
--				end
--			end
--		end
--	end
--
--	local cast = ffi.cast(clEvHook, plman.clientEvHook.hookCtx.orig)
--
--	ret = cast(ptr, event)
--
--	plman.clientEvHook:replace()
--
--	return ret
--end
--)
