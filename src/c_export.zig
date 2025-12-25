const std = @import("std");

const builtin = @import("builtin");
const native_os = builtin.os.tag;

const asy = @import("helpers/zig_async/async.zig");
const hlp = @import("helpers/utils.zig");

export fn cexp_debugPrint(str: [*c]const u8) void {
    std.debug.print("{s}\n", .{str});
}

export fn cexp_zigsleep(ns: u64) void {
    std.Thread.sleep(ns);
}

/// std.Thread.sleep(ns) but for ms
export fn cexp_sleep_ms(ms: u64) void {
    const s = ms / std.time.ms_per_s;
    const ns = (ms % std.time.ms_per_s) * std.time.ns_per_ms;

    switch (native_os) {
        .linux => {
            const linux = std.os.linux;

            var req: linux.timespec = .{
                .sec = std.math.cast(linux.time_t, s) orelse std.math.maxInt(linux.time_t),
                .nsec = std.math.cast(linux.time_t, ns) orelse std.math.maxInt(linux.time_t),
            };
            var rem: linux.timespec = undefined;

            while (true) {
                switch (linux.E.init(linux.clock_nanosleep(.MONOTONIC, .{ .ABSTIME = false }, &req, &rem))) {
                    .SUCCESS => return,
                    .INTR => {
                        req = rem;
                        continue;
                    },
                    .FAULT,
                    .INVAL,
                    .OPNOTSUPP,
                    => unreachable,
                    else => return,
                }
            }
        },

        .windows => {
            const ms32 = std.math.cast(std.os.windows.DWORD, ms) orelse std.math.maxInt(std.os.windows.DWORD);

            std.os.windows.kernel32.Sleep(ms32);
        },

        else => {
            std.posix.nanosleep(s, ns);
        },
    }
}

export fn cexp_allocLoop() ?*asy.Loop {
    return hlp.gpa.create(asy.Loop) catch null;
}

export fn cexp_freeLoop(loop: *asy.Loop) void {
    hlp.gpa.destroy(loop);
}

export fn cexp_initLoop(loop: *asy.Loop) void {
    loop.init();
}

export fn cexp_deinitLoop(loop: *asy.Loop) void {
    loop.deinit();
}

export fn cexp_callLoop(loop: *asy.Loop) void {
    loop.call(hlp.gpa) catch {};
}

export fn cexp_callLoopSameThread(loop: *asy.Loop) void {
    loop.callSameThread(hlp.gpa);
}

export fn cexp_execLoop(loop: *asy.Loop) void {
    loop.exec();
}

export fn cexp_pushToLoop(loop: *asy.Loop, ptr: *anyopaque, args: [*c]*anyopaque, len: usize) void {
    loop.add(hlp.gpa, .{
        .ptr = ptr,
        .args = if (len == 0) &[_]*anyopaque{} else args[0..len],
    }) catch {};
}

export fn cexp_asySpawn(ptr: *anyopaque, args: [*c]*anyopaque, len: usize) void {
    asy.Spawn(asy.callFunc, .{
        asy.FuncData{
            .ptr = ptr,
            .args = if (len == 0) &[_]*anyopaque{} else args[0..len],
        },
    }) catch {};
}

export fn cexp_allocResetEvent() ?*std.Thread.ResetEvent {
    return hlp.gpa.create(std.Thread.ResetEvent) catch null;
}

export fn cexp_freeResetEvent(resetEvent: *std.Thread.ResetEvent) void {
    hlp.gpa.destroy(resetEvent);
}

export fn cexp_waitResetEvent(resetEvent: *std.Thread.ResetEvent) void {
    resetEvent.wait();
}

export fn cexp_timedWaitResetEvent(resetEvent: *std.Thread.ResetEvent, timeout_ns: u64) bool {
    resetEvent.timedWait(timeout_ns) catch {
        return true;
    };

    return false;
}

export fn cexp_setResetEvent(resetEvent: *std.Thread.ResetEvent) void {
    resetEvent.set();
}

export fn cexp_resetResetEvent(resetEvent: *std.Thread.ResetEvent) void {
    resetEvent.reset();
}

export fn cexp_timeMicroTimestamp() i64 {
    return std.time.microTimestamp();
}

export fn cexp_allocTimer() ?*std.time.Timer {
    return hlp.gpa.create(std.time.Timer) catch null;
}

export fn cexp_startTimer() ?*std.time.Timer {
    const timer = hlp.gpa.create(std.time.Timer) catch return null;

    const current = std.time.Instant.now() catch {
        hlp.gpa.destroy(timer);

        return null;
    };

    timer.previous = current;
    timer.started = current;

    return timer;
}

export fn cexp_freeTimer(timer: *std.time.Timer) void {
    hlp.gpa.destroy(timer);
}

export fn cexp_readTimer(timer: *std.time.Timer) u64 {
    return timer.read();
}

export fn cexp_lapTimer(timer: *std.time.Timer) u64 {
    return timer.lap();
}

export fn cexp_resetTimer(timer: *std.time.Timer) void {
    return timer.reset();
}

export fn cexp_allocMutex() ?*std.Thread.Mutex {
    return hlp.gpa.create(std.Thread.Mutex) catch null;
}

export fn cexp_freeMutex(mutex: *std.Thread.Mutex) void {
    hlp.gpa.destroy(mutex);
}

export fn cexp_lockMutex(mutex: *std.Thread.Mutex) void {
    mutex.lock();
}

export fn cexp_tryLockMutex(mutex: *std.Thread.Mutex) bool {
    return mutex.tryLock();
}

export fn cexp_unlockMutex(mutex: *std.Thread.Mutex) void {
    mutex.unlock();
}

//export fn cexp_bitAND(a1: i32, a2: i32) i32 {
//    return a1 & a2;
//}
//
//export fn cexp_bit64AND(a1: i64, a2: i64) i64 {
//    return a1 & a2;
//}
//
//export fn cexp_bitOR(a1: i32, a2: i32) i32 {
//    return a1 | a2;
//}
//
//export fn cexp_bit64OR(a1: i64, a2: i64) i64 {
//    return a1 | a2;
//}
//
//export fn cexp_bitXOR(a1: i32, a2: i32) i32 {
//    return a1 ^ a2;
//}
//
//export fn cexp_bit64XOR(a1: i64, a2: i64) i64 {
//    return a1 ^ a2;
//}
//
//export fn cexp_bitNOT(a1: i32) i32 {
//    return ~a1;
//}
//
//export fn cexp_bit64NOT(a1: i64) i64 {
//    return ~a1;
//}
//
//export fn cexp_bitLSH(a1: i32, a2: i32) i32 {
//    return a1 << std.math.lossyCast(u5, a2);
//}
//
//export fn cexp_bit64LSH(a1: i64, a2: i64) i64 {
//    return a1 << std.math.lossyCast(u6, a2);
//}
//
//export fn cexp_bitRSH(a1: i32, a2: i32) i32 {
//    return a1 >> std.math.lossyCast(u5, a2);
//}
//
//export fn cexp_bit64RSH(a1: i64, a2: i64) i64 {
//    return a1 >> std.math.lossyCast(u6, a2);
//}
