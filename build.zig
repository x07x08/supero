const std = @import("std");
const bshr = @import("src/build_shared.zig");
const z2z = @import("src/helpers/s2s/zig2zig.zig");
const CLib = @import("src/helpers/extlib/extlib.zig").CLib;

pub const LoaderInfo = struct {
    pub var LibOutMode: i8 = @intFromEnum(bshr.LoaderType.Lib);
};

const cPath = "external/c/";

fn libmemBuildFiles(b: *std.Build, target: std.Build.ResolvedTarget) []const []const u8 {
    var ret: []const []const u8 = &.{};

    switch (target.result.os.tag) {
        .windows => {
            switch (target.result.cpu.arch) {
                .x86, .x86_64 => {
                    ret = std.mem.concat(b.allocator, []const u8, &.{
                        ret,
                        &.{
                            cPath ++ "libmem/src/common/arch/x86.c",
                        },
                    }) catch unreachable;
                },

                .aarch64 => {
                    ret = std.mem.concat(b.allocator, []const u8, &.{
                        ret,
                        &.{
                            cPath ++ "libmem/src/common/arch/aarch64.c",
                        },
                    }) catch unreachable;
                },

                else => {
                    ret = std.mem.concat(b.allocator, []const u8, &.{
                        ret,
                        &.{
                            cPath ++ "libmem/src/common/arch/generic.c",
                        },
                    }) catch unreachable;
                },
            }

            ret = std.mem.concat(b.allocator, []const u8, &.{
                ret,
                &.{
                    cPath ++ "libmem/internal/winutils/winutils.c",
                },
            }) catch unreachable;

            ret = std.mem.concat(b.allocator, []const u8, &.{
                ret,
                &.{
                    cPath ++ "libmem/src/win/memory.c",
                    cPath ++ "libmem/src/win/module.c",
                    cPath ++ "libmem/src/win/osprot.c",
                    cPath ++ "libmem/src/win/process.c",
                    cPath ++ "libmem/src/win/segment.c",
                    cPath ++ "libmem/src/win/symbol.c",
                    cPath ++ "libmem/src/win/thread.c",
                },
            }) catch unreachable;

            ret = std.mem.concat(b.allocator, []const u8, &.{
                ret,
                &.{
                    cPath ++ "libmem/src/common/asm.c",
                    cPath ++ "libmem/src/common/memory.c",
                    cPath ++ "libmem/src/common/module.c",
                    cPath ++ "libmem/src/common/process.c",
                    cPath ++ "libmem/src/common/scan.c",
                    cPath ++ "libmem/src/common/segment.c",
                    cPath ++ "libmem/src/common/symbol.c",
                    cPath ++ "libmem/src/common/thread.c",
                    cPath ++ "libmem/src/common/vmt.c",
                },
            }) catch unreachable;
        },

        .linux => {
            switch (target.result.cpu.arch) {
                .x86 => {
                    ret = std.mem.concat(b.allocator, []const u8, &.{
                        ret,
                        &.{
                            cPath ++ "libmem/src/common/arch/x86.c",
                            cPath ++ "libmem/src/linux/ptrace/x86/x86.c",
                        },
                    }) catch unreachable;
                },

                .x86_64 => {
                    ret = std.mem.concat(b.allocator, []const u8, &.{
                        ret,
                        &.{
                            cPath ++ "libmem/src/common/arch/x86.c",
                            cPath ++ "libmem/src/linux/ptrace/x64/x64.c",
                        },
                    }) catch unreachable;
                },

                .aarch64 => {
                    ret = std.mem.concat(b.allocator, []const u8, &.{
                        ret,
                        &.{
                            cPath ++ "libmem/src/common/arch/aarch64.c",
                            cPath ++ "libmem/src/linux/ptrace/aarch/aarch64.c",
                        },
                    }) catch unreachable;
                },

                else => {
                    ret = std.mem.concat(b.allocator, []const u8, &.{
                        ret,
                        &.{
                            cPath ++ "libmem/src/common/arch/generic.c",
                            cPath ++ "libmem/src/linux/ptrace/generic/generic.c",
                        },
                    }) catch unreachable;
                },
            }

            ret = std.mem.concat(b.allocator, []const u8, &.{
                ret,
                &.{
                    cPath ++ "libmem/src/linux/ptrace/ptrace.c",
                    cPath ++ "libmem/internal/posixutils/posixutils.c",
                    cPath ++ "libmem/internal/elfutils/elfutils.c",
                },
            }) catch unreachable;

            ret = std.mem.concat(b.allocator, []const u8, &.{
                ret,
                &.{
                    cPath ++ "libmem/src/linux/memory.c",
                    cPath ++ "libmem/src/linux/module.c",
                    cPath ++ "libmem/src/linux/process.c",
                    cPath ++ "libmem/src/linux/segment.c",
                    cPath ++ "libmem/src/linux/symbol.c",
                    cPath ++ "libmem/src/linux/thread.c",
                    cPath ++ "libmem/src/linux/utils.c",
                },
            }) catch unreachable;

            ret = std.mem.concat(b.allocator, []const u8, &.{
                ret,
                &.{
                    cPath ++ "libmem/src/common/asm.c",
                    cPath ++ "libmem/src/common/memory.c",
                    cPath ++ "libmem/src/common/module.c",
                    cPath ++ "libmem/src/common/process.c",
                    cPath ++ "libmem/src/common/scan.c",
                    cPath ++ "libmem/src/common/segment.c",
                    cPath ++ "libmem/src/common/symbol.c",
                    cPath ++ "libmem/src/common/thread.c",
                    cPath ++ "libmem/src/common/vmt.c",
                },
            }) catch unreachable;
        },

        else => {},
    }

    return ret;
}

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});
    const strippdb = b.option(bool, "strippdb", "Strip debug symbols file") orelse (optimize != .Debug);

    const loaderMode = b.option(bshr.LoaderType, "loadermode", "Output mode for the loader.") orelse .Lib;
    LoaderInfo.LibOutMode = @intFromEnum(loaderMode);
    const isExecutable = (loaderMode == .Exe);

    const loaderInfo = z2z.serializeType(LoaderInfo, b.allocator,
        \\const std = @import("std");
        \\
        \\
    ) catch unreachable;

    const genFile = b.addWriteFiles();
    const lshr = genFile.add("loader_shared.zig", loaderInfo);

    var libSource: []const u8 = "src/lib_common.zig";
    const libName: []const u8 = bshr.libName;

    const os = target.result.os.tag;

    switch (os) {
        .windows => {
            libSource = "src/lib_internal_win32.zig";
        },

        .linux => {
            if (target.query.abi != .gnu) {
                std.log.info("Non GNU ABI is incompatible", .{});
            }

            libSource = "src/lib_internal_linux.zig";
        },

        else => {},
    }

    if (isExecutable) {
        libSource = "src/lib_executable.zig";
    }

    const lib = b.createModule(.{
        .root_source_file = b.path(libSource),
        .target = target,
        .optimize = optimize,
        .strip = strippdb,
        .link_libc = true,
    });
    lib.addAnonymousImport("loader_shared", .{
        .root_source_file = lshr,
    });

    const libBin = if (!isExecutable)
        b.addLibrary(.{
            .linkage = .dynamic,
            .name = libName,
            .root_module = lib,
            .use_llvm = true,
            .use_lld = true,
        })
    else
        b.addExecutable(.{
            .linkage = .dynamic,
            .name = libName ++ "_bin",
            .root_module = lib,
            .use_llvm = true,
            .use_lld = true,
        });
    libBin.root_module.addIncludePath(b.path(cPath));
    libBin.root_module.addIncludePath(b.path("src/lua/include/tf2_headers"));

    // Mandatory for Lua
    libBin.rdynamic = true;

    if (os == .linux) {
        if (target.query.cpu_arch == .x86) {
            libBin.link_z_notext = true;
        }
    }

    var libmem = CLib{
        .b = b,
        .inc_path = &.{
            cPath,
            cPath ++ "libmem/include",
            cPath ++ "libmem/include/libmem",
            cPath ++ "libmem/internal",
            cPath ++ "libmem/internal/elfutils",
            cPath ++ "libmem/internal/posixutils",
            cPath ++ "libmem/internal/winutils",
            cPath ++ "libmem/src",
            cPath ++ "libmem/src/common",
            cPath ++ "libmem/src/common/arch",
            cPath ++ "libmem/src/linux",
            cPath ++ "libmem/src/linux/ptrace/generic",
            cPath ++ "libmem/src/linux/ptrace/x64",
            cPath ++ "libmem/src/linux/ptrace/x86",
            cPath ++ "libmem/src/linux/ptrace/aarch64",
            cPath ++ "libmem/src/win",
        },
        .name = "mem",
        .target = target,
        .optimize = optimize,
        .strip = strippdb,
        .c_files = libmemBuildFiles(b, target),
        .cflags = &.{
            "-fno-sanitize=undefined",
            "-Wno-date-time",
            "-Wno-incompatible-pointer-types",
            "-Wno-int-conversion",
        },
    };

    // Can't get symbols to be exported

    libmem.dynamic = true;

    const libmem_bin = libmem.makeLibrary();

    libBin.root_module.linkLibrary(libmem_bin);

    var libdetour = CLib{
        .b = b,
        .inc_path = &.{
            cPath,
        },
        .name = "detour",
        .target = target,
        .optimize = optimize,
        .strip = strippdb,
        .c_files = &.{
            cPath ++ "libdetour/libdetour.c",
        },
        .cflags = &.{
            "-fno-sanitize=undefined",
            "-Wno-date-time",
            "-Wno-incompatible-pointer-types",
            "-Wno-int-conversion",
        },
    };

    libdetour.dynamic = true;

    const libdetour_bin = libdetour.makeLibrary();

    libBin.root_module.linkLibrary(libdetour_bin);

    const luaJIT = b.dependency("luajit_build", .{
        .target = target,
        .optimize = optimize,
        .link_as = .static,
    });

    const luaJITBin = luaJIT.module("luajit-build");

    libBin.root_module.addImport("clj", luaJITBin);

    b.getInstallStep().dependOn(&b.addInstallArtifact(libBin, .{
        .implib_dir = .disabled,
    }).step);
}
