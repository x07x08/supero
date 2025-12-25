-- Comment to change the size and reexecute

local tf = gModules.tf

local inst = tf.mem.inst

local hICvar = inst.cvar

local loadedCode, loadErr

loadedCode, loadErr = loadfile(__LUA_SCRIPT_PATH .. "tf/utils.lua")

if (not loadedCode) then
	print(loadErr)

	return nil
end

---@module "tf/utils"
local utils = loadedCode()

---@param ev asy_EventLoop
---@param ownPlugin tfT_Plugin
---@param plman tf_PluginsManager
local function main(ev, ownPlugin, plman)
	hICvar.vmt.ConsoleColorPrintf(hICvar, utils.ConColors.yellow, "[Supero] \"%s\" (re)loaded\n", ownPlugin.name)
end

collectgarbage()

return main
