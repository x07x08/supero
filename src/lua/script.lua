local function getScriptPath()
	local ret = os.getenv("LUA_SCRIPT_PATH")

	if (ret == nil) then
		return nil
	end

	return ret:match(".*[/\\]")
end

__LIB_NAME = "supero"

__LUA_SCRIPT_PATH = getScriptPath() or (__LIB_NAME .. "/lua/")

require(__LUA_SCRIPT_PATH .. "runtime/globals")

local lcpp = gModules.lcpp
local asy = gModules.asy

local fn = __fn

table.insert(lcpp.INCLUDE_PATHS, __LUA_SCRIPT_PATH .. "include/libmem")
table.insert(lcpp.INCLUDE_PATHS, __LUA_SCRIPT_PATH .. "include/tf2_headers")

local localLibPath = function(name)
	local moduleName = name .. __DYN_LIB_SUF

	if (ffi.os == "Windows") then
		return moduleName
	elseif (ffi.os == "Linux") then
		return "./" .. moduleName
	end
end

local loaderType = __loaderType

CExt.self = ffi.C
CExt.lmem = ffi.load(localLibPath("libmem"))
CExt.ldtour = ffi.load(localLibPath("libdetour"))

if (loaderType ~= 0) then
	local selfLib = ffi.load(localLibPath(__LIB_NAME))

	if (ffi.os == "Linux") then
		CExt.self = selfLib
	end
end

local incDecl = __ARCHDECL .. __OSDECL .. [[
typedef struct PlugObjSPO
{
	void* vmt;
	void* ptr;
} PlugObjSPO;

PlugObjSPO g_plugSPO;

#define TF_CLIENT_DLL
#define TF_DLL
#define NEXT_BOT

#include "supero/lua/include/libdetour.h"
#include "supero/lua/include/libmem/libmem.h"
#include "supero/lua/include/tf2_headers/SDK.h"
]]

ffi.cdef(incDecl)

---@module "tf/tf"
gModules.tf = require(__LUA_SCRIPT_PATH .. "tf/tf")

local tf = gModules.tf

print("Lua loaded")

---@type ffi.cdata* | nil
local currentProc = ffi.new("lm_process_t")

local function getProcess()
	if (CExt.lmem.LM_GetProcess(currentProc) == ffi.C.LM_FALSE) then
		currentProc = nil
	end

	return (currentProc ~= nil)
end

local function getModules()
	local found = false

	for k in pairs(tf.mem.libs) do
		tf.mem.libs[k] = ffi.new("lm_module_t")
		found = CExt.lmem.LM_FindModuleEx(currentProc, k .. __DYN_LIB_SUF, tf.mem.libs[k])

		if (found == ffi.C.LM_FALSE) then
			tf.mem.libs[k] = nil
		end
	end
end

local function getInterfaceInstances()
	for _, v in pairs(tf.mem.intfs) do
		v:getInstance(tf.mem.libs[v.cModule])
	end
end

local plman = tf.PluginsManager:init({})

local svEvHook = ffi.typeof("bool (*)(IGameEventManager2*, IGameEvent*, bool)")
local clEvHook = ffi.typeof("bool (*)(IGameEventManager2*, IGameEvent*)")

-- If this is not first, the FireEvent hook below will crash.
local __FireEventHookFix = ffi.cast(svEvHook, function(ptr, event, dontBroadcast)

end
)

local FireEventHook = ffi.cast(svEvHook, function(ptr, event, dontBroadcast)
	plman.serverEvHook:restore()

	local eventName = ffi.string(event.vmt.GetName(event))
	local refDontBroadcast = ffi.new("bool[1]", dontBroadcast)
	local funcsList = nil
	local ret = nil

	for _, pluginEvents in pairs(plman.eventsList) do
		funcsList = pluginEvents[eventName]

		if (funcsList) then
			for _, cb in ipairs(funcsList) do
				ret = cb(ptr, event, refDontBroadcast)

				if ((ret ~= nil) and (ret >= ffi.C.PLUGIN_STOP)) then
					return false
				end
			end
		end
	end

	local cast = ffi.cast(svEvHook, plman.serverEvHook.hookCtx.orig)

	ret = cast(ptr, event, refDontBroadcast[0])

	plman.serverEvHook:replace()

	return ret
end
)

local FireEventClientHook = ffi.cast(clEvHook, function(ptr, event)
	plman.clientEvHook:restore()

	local eventName = ffi.string(event.vmt.GetName(event))
	local funcsList = nil
	local ret = nil

	for _, pluginEvents in pairs(plman.clientEventsList) do
		funcsList = pluginEvents[eventName]

		if (funcsList) then
			for _, cb in ipairs(funcsList) do
				ret = cb(ptr, event)

				if (ret >= ffi.C.PLUGIN_STOP) then
					return false
				end
			end
		end
	end

	local cast = ffi.cast(clEvHook, plman.clientEvHook.hookCtx.orig)

	ret = cast(ptr, event)

	plman.clientEvHook:replace()

	return ret
end
)

local scriptFile = __LIB_NAME .. "/lua/main.lua"

local isClientPlugin = false

local isClientVSP = ((loaderType == 2) and (isClientPlugin == true) and (ffi.os == "Linux"))

local minSDiv = fn(function()
	--if (isClientVSP) then
	--	return asy.us_per_s
	--end

	return asy.ns_per_s
end)

---@param ev asy_EventLoop
local function main(ev)
	if (not getProcess()) then
		error("Could not find TF2.")
	end

	getModules()
	getInterfaceInstances()

	--local gVB = tf.mem.intfs.globalVarsBase

	--if (gVB.instance) then
		--gVB.instance = ffi.cast(gVB.typeStr .. "* (*)(void*)", gVB.instance)(gVB.instance)
	--end

	tf.initInst()

	--if (tf.mem.inst.gameEventManager ~= nil) then
		--plman:initHooks(FireEventHook, FireEventClientHook)

		--plman.serverEvHook:replace();

		-- Until it gets in the menu
		-- It seems to crash because of too much of something...
		--if (loaderType == 2) then
			--ev:timeout(plman.clientEvHook.replace, 10 * minSDiv, plman.clientEvHook)
		--else
			--plman.clientEvHook:replace();
		--end
	--end

	local mainPlugin = tf.Types.Plugin:init({}, scriptFile, "main")

	if (plman:getOrAddPlugin(mainPlugin) == nil) then
		ev:periodic(plman.updateScripts, 0.5 * minSDiv, plman, ev, mainPlugin, plman)
	end
end

local ev = fn(function()
	--if (isClientVSP) then
	--	return tf.Types.ClVSPEventLoop:init({})
	--end

	return asy.EventLoop:init({})
end)
ev:timeout(main, 0, ev)
ev:start()

print("Loop ended")
