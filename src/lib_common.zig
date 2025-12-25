const std = @import("std");

const builtin = @import("builtin");
const native_os = builtin.os.tag;

const bshr = @import("build_shared.zig");

const hlp = @import("helpers/utils.zig");
const z2l = @import("helpers/s2s/zig2lua.zig");
const asy = @import("helpers/zig_async/async.zig");

const lshr = @import("loader_shared");

const clj = @import("clj");

const c = @cImport({
    @cInclude("libmem/include/libmem/libmem.h");
    @cInclude("libdetour/libdetour.h");
    @cInclude("Interfaces/IServerPlugin.h");
});

pub const Errors = error{
    LuaContextFailed,
};

fn fakeUse(arg: anytype) void {
    _ = arg;
}

export fn __fixbrokenexport() *const anyopaque {
    _ = @import("c_export.zig");

    fakeUse(&c.LM_EnumThreads);
    fakeUse(&c.LM_EnumSegments);
    fakeUse(&c.LM_EnumModules);
    fakeUse(&c.LM_SetMemory);
    fakeUse(&c.LM_DeepPointer);
    fakeUse(&c.LM_GetArchitecture);
    fakeUse(&c.LM_VmtHook);
    fakeUse(&c.LM_SigScan);
    fakeUse(&c.LM_FindSymbolAddress);
    fakeUse(&c.detour_enable);

    return @ptrCast(&null);
}

fn plugLoad(ptr: ?*anyopaque, ciFn: ?*anyopaque, gsFn: ?*anyopaque) callconv(.c) bool {
    _ = ptr;
    _ = ciFn;
    _ = gsFn;

    lshr.LibOutMode = @intFromEnum(bshr.LoaderType.VSP);

    SuperoMain();

    return true;
}

fn plugRetCont() callconv(.c) c_int {
    return c.PLUGIN_CONTINUE;
}

fn plugDesc() callconv(.c) [*c]const u8 {
    const ret: [:0]const u8 = "Supero : LuaJIT plugin system";

    return ret.ptr;
}

fn plugEmptyFn(ptr: *anyopaque) callconv(.c) void {
    _ = ptr;
}

const PlugVMT = extern struct {
    vmt_IServerPluginCallbacks: c.IServerPluginCallbacks_VMT = .{
        .Load = @ptrCast(&plugLoad),
        .ClientConnect = @ptrCast(&plugRetCont),
        .ClientCommand = @ptrCast(&plugRetCont),
        .NetworkIDValidated = @ptrCast(&plugRetCont),
        .GetPluginDescription = @ptrCast(&plugDesc),
    },
};

var spoVMT: PlugVMT = .{};

const PlugObj = extern struct {
    vmt: *PlugVMT = &spoVMT,
};

export var g_plugSPO: PlugObj = .{};

export fn CreateInterface(name: [*c]const u8, outRet: ?*c_int) ?*anyopaque {
    if (std.mem.orderZ(u8, name, c.IServerPluginCallbacks_SIGNATURE) == .eq) {
        if (outRet) |val| {
            val.* = 0;
        }

        return &g_plugSPO;
    }

    if (outRet) |val| {
        val.* = 1;
    }

    return null;
}

pub fn mainThread() void {
    var exit = false;

    start(&exit) catch |err| {
        const sl = @src();

        std.log.err("{s}->{s}->L:{d} : {}", .{ sl.file, sl.fn_name, sl.line, err });
    };

    if (exit) std.process.exit(0);
}

pub export fn SuperoMain() void {
    asy.Spawn(mainThread, .{}) catch {};
}

fn startLua(luaCtx: *clj.lua_State, settings: *std.process.EnvMap) void {
    z2l.containerToTable(luaCtx, @typeInfo(std.fs.path), "__stdfspath", std.fs.path);

    const lsVar = settings.get("LUA_SCRIPT_PATH");
    const luaScript = lsVar orelse bshr.libName ++ "/lua/script.lua";

    const err = clj.luaL_dofile(luaCtx, luaScript.ptr);

    if (err) {
        std.log.err("{s}", .{
            clj.lua_tolstring(luaCtx, -1, null),
        });

        clj.lua_pop(luaCtx, 1);
    }
}

fn start(exit: *bool) !void {
    hlp.initAllocator();

    var settings = try std.process.getEnvMap(hlp.gpa);

    defer _ = hlp.deinitAllocator();

    defer {
        settings.deinit();
    }

    defer {
        const eoleVar = settings.get("EXIT_ON_LOOP_END") orelse "0";
        const exitOnLoopEnd: bool = ((std.fmt.parseInt(i8, eoleVar, 10) catch 0) == 1);

        if (exitOnLoopEnd) {
            exit.* = true;
        }
    }

    if (native_os == .windows) {
        const winDefs = @import("win32/defines.zig");

        if (settings.get("DEBUG")) |debug| {
            if (((std.fmt.parseInt(i8, debug, 10) catch 0) == 1)) {
                const conErr = winDefs.AllocConsole();

                if (conErr == 0) {
                    winDefs.MessageBoxA_Util(
                        hlp.gpa,
                        "Console error",
                        .{},
                        "{s}",
                        .{bshr.windowName},
                        0x10,
                    ) catch unreachable;
                }
            }
        }
    }

    std.log.info("Supero loaded", .{});

    const luaCtx = clj.luaL_newstate() orelse return Errors.LuaContextFailed;

    clj.luaL_openlibs(luaCtx);

    const cwd = std.fs.cwd().realpathAlloc(hlp.gpa, ".") catch unreachable;
    z2l.registerValue(luaCtx, []u8, "__currentDirectory", cwd);
    z2l.registerValue(luaCtx, i8, "__loaderType", lshr.LibOutMode);

    hlp.gpa.free(cwd);

    startLua(luaCtx, &settings);

    clj.lua_close(luaCtx);
}
