#pragma once

#define TYPE_ON_SZ(sign, type, prefix, bits) typedef type sign int prefix##int##bits##_t
namespace libc {
	typedef char int8_t;
	typedef unsigned char uint8_t;

	TYPE_ON_SZ( , short, , 16);
	TYPE_ON_SZ( ,      , , 32);
	TYPE_ON_SZ( ,  long, , 64);

	TYPE_ON_SZ(unsigned, short, u, 16);
	TYPE_ON_SZ(unsigned,      , u, 32);
	TYPE_ON_SZ(unsigned,  long, u, 64);

	typedef uint64_t size_t;
	typedef __builtin_va_list va_list;
	typedef long __ssize_t;
	typedef __ssize_t ssize_t;
}
#ifndef va_start
#define va_start(ap, param) __builtin_va_start(ap, param)
#define va_end(ap) __builtin_va_end(ap)
#define va_arg(ap, type) __builtin_va_arg(ap, type)
#define va_copy(dest, src) __builtin_va_copy(dest, src)
#endif

#ifndef NULL
# define NULL (void*)0
#endif

#undef TYPE_ON_SZ
