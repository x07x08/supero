const std = @import("std");

export const spolib: *const anyopaque = @ptrCast(@alignCast(&@import("lib_common.zig")));
