-- __currentDirectory - The current directory of the binary
-- __stdfspath - Zig's std.fs.path
-- __loaderType - 0 = Exe, 1 = Lib, 2 = VSP

---@return any
__MakeClass = __MakeClass or function(class, bTable)
	setmetatable(bTable, class)
	class.__index = class

	return bTable
end

__fn = __fn or function(func)
	return func()
end

__LUA_SCRIPT_PATH = __LUA_SCRIPT_PATH or ""

gModules = gModules or {}

---@module "lcpp"
gModules.lcpp = gModules.lcpp or require(__LUA_SCRIPT_PATH .. "lcpp/lcpp")
gModules.lcpp.cParser = gModules.lcpp.init("", ffi.lcpp_defs)
gModules.lcpp.INCLUDE_PATHS = {
	__LUA_SCRIPT_PATH .. "include",
}

CExt = {}

__DYN_LIB_SUF = __DYN_LIB_SUF or __fn(function()
	if (ffi.os == "Windows") then
		return ".dll"
	elseif (ffi.os == "Linux") then
		return ".so"
	end
end)

__ARCHDECL = __ARCHDECL or __fn(function()
	if (ffi.abi("32bit")) then
		return "#define __i386__\n"
	end

	return "#define __x86_64__\n"
end)

__OSDECL = __OSDECL or __fn(function()
	if (ffi.os == "Windows") then
		return "#define _WIN32\n#define _WIN64\n"
	elseif (ffi.os == "Linux") then
		return "#define __linux__\n"
	end
end)

ffi.cdef [[
#include "zlj_runtime.h"
]]

gModules.bitOps = gModules.bitOps or require("bit")

---@module "runtime/utils"
gModules.rtUtils = gModules.rtUtils or require(__LUA_SCRIPT_PATH .. "runtime/utils")

---@module "asy"
gModules.asy = gModules.asy or require(__LUA_SCRIPT_PATH .. "runtime/asy")

---@module "runtime/pluginsys"
gModules.pluginSys = gModules.pluginSys or require(__LUA_SCRIPT_PATH .. "runtime/pluginsys")
