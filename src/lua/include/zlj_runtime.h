#if !defined(CLibzlj_runtime_Included)
#define CLibzlj_runtime_Included

#if defined(__INTELLISENSE__) || defined(__JETBRAINS_IDE__)
#include <stdbool.h>
#include <stdint.h>
#include <wchar.h>
#endif

void cexp_debugPrint(const char* str);

void cexp_zigsleep(uint64_t ns);
void cexp_sleep_ms(uint64_t ms);

void* cexp_allocLoop();
void cexp_freeLoop(void* loop);
void cexp_initLoop(void* loop);
void cexp_deinitLoop(void* loop);
void cexp_callLoop(void* loop);
void cexp_callLoopSameThread(void* loop);
void cexp_execLoop(void* loop);
void cexp_pushToLoop(void* loop, void (*ptr)(), void* args, size_t len);

void cexp_asySpawn(void (*ptr)(), void* args, size_t len);

void* cexp_allocResetEvent();
void cexp_freeResetEvent(void* resetEvent);
void cexp_waitResetEvent(void* resetEvent);
bool cexp_timedWaitResetEvent(void* resetEvent, uint64_t timeout_ns);
void cexp_setResetEvent(void* resetEvent);
void cexp_resetResetEvent(void* resetEvent);

int64_t cexp_timeMicroTimestamp();
void* cexp_allocTimer();
void* cexp_startTimer();
void cexp_freeTimer(void* timer);
uint64_t cexp_readTimer(void* timer);
uint64_t cexp_lapTimer(void* timer);
void cexp_resetTimer(void* timer);

void* cexp_allocMutex();
void cexp_freeMutex(void* mutex);
void cexp_lockMutex(void* mutex);
bool cexp_tryLockMutex(void* mutex);
void cexp_unlockMutex(void* mutex);

//int cexp_bitAND(int a1, int a2);
//int64_t cexp_bit64AND(int64_t a1, int64_t a2);
//int cexp_bitOR(int a1, int a2);
//int64_t cexp_bit64OR(int64_t a1, int64_t a2);
//int cexp_bitXOR(int a1, int a2);
//int64_t cexp_bit64XOR(int64_t a1, int64_t a2);
//int cexp_bitNOT(int a1);
//int64_t cexp_bit64NOT(int64_t a1);
//int cexp_bitLSH(int a1, int a2);
//int64_t cexp_bit64LSH(int64_t a1, int64_t a2);
//int cexp_bitRSH(int a1, int a2);
//int64_t cexp_bit64RSH(int64_t a1, int64_t a2);

#endif
