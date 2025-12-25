const std = @import("std");

const winDefs = @import("win32/defines.zig");
const winAPI = std.os.windows;

const asy = @import("helpers/zig_async/async.zig");

export const spolib: *const anyopaque = @ptrCast(@alignCast(&@import("lib_common.zig")));

//const spolib = @import("lib_common.zig");
//
//pub fn DllMain(
//    hinstDLL: *anyopaque,
//    dwReason: winAPI.DWORD,
//    lpReserved: winAPI.LPVOID,
//) callconv(.winapi) winAPI.BOOL {
//    _ = lpReserved;
//    _ = hinstDLL;
//
//    if (dwReason == winDefs.DLL_PROCESS_ATTACH) {
//        spolib.SuperoMain();
//    }
//
//    return winAPI.TRUE;
//}
