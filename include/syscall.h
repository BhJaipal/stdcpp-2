#ifndef MY_SYSCALL_H
#define MY_SYSCALL_H
#include "types.h"

#if __cplusplus
#define EXPORT extern "C" {
#define EXPORT_END }
#else
#define EXPORT
#define EXPORT_END
#endif

#define SYSCALL(ret, rax, ...) \
	size_t argv[] = {__VA_ARGS__, 0, 0, 0, 0, 0};\
	ret syscall(argv[0], argv[1], argv[2], argv[3], argv[4], argv[5], rax);

size_t syscall(long rdi, long rsi, long rdx, long r10, long r8, long r9, long rax);

#if defined __x86_64 || defined __x86_64__
#define RAX "rax"
#define RDI "rdi"
#define RSI "rsi"
#define RDX "rdx"
#define R10 "r10"
#define R8   "r8"
#define R9   "r9"
#define R11  "r11"
#define SYSCALL_EXEC asm("syscall\n");
#else
#define RAX "eax"
#define RDI "ebx"
#define RSI "ecx"
#define RDX "edx"
#define R10 "esi"
#define R8  "edi"
#define R9  "ebp"
#define R11  "r11d"
#define SYSCALL_EXEC asm("\nint 0x80\n");
#endif

#endif // !MY_SYSCALL_H
