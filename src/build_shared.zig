pub const libName: []const u8 = "supero";
pub const windowName: [:0]const u8 = "Supero";

pub const LoaderType = enum(i8) {
    Exe = 0,
    Lib,
    VSP,
};
