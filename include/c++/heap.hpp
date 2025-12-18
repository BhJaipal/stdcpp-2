#pragma once
#include "libc.hpp"

namespace libc {
	typedef enum {
		PROT_READ = 0x1,		/* Page can be read.  */
		PROT_WRITE= 0x2,		/* Page can be written.  */
		PROT_EXEC = 0x4,		/* Page can be executed.  */
		PROT_NONE = 0x0,		/* Page can not be accessed.  */
		PROT_GROWSDOWN =	0x01000000, /* Extend change to start of growsdown vma (mprotect only).  */
		PROT_GROWSUP	=	0x02000000, /* Extend change to start of growsup vma (mprotect only).  */
	} PageProtection;

	/* Sharing types (must choose one and only one of these).  */
	typedef enum {
		MAP_SHARED =	0x01,		/* Share changes.  */
		MAP_PRIVATE=	0x02,		/* Changes are private.  */
		MAP_FIXED = 	0x10,		/* Interpret addr exactly.  */
		MAP_ANONYMOUS = 0x20,		/* Don't use a file.  */
	} MapProps;

	extern "C" {
		void *mmap (void *addr, size_t len, int prot,
				int flags, int fd, long offset);
		int mprotect(void *__addr, size_t __len, int __prot);
		void munmap(void *ptr, size_t size);
		void memcpy(void* dest, void *src, libc::size_t n);
	}
}

class Heap {
	typedef enum {
		HeapAlloc,
		HeapDealloc,
	} HeapEvent;

	struct Ptr {
		Ptr *next;
		void *ptr;
		libc::size_t size;
	};

	Ptr *head;
	Ptr *end;
	libc::size_t node_size;
	libc::PageProtection prot;
	libc::MapProps flags;
	int fd;

public:
	Heap(int prot_ = libc::PageProtection::PROT_READ | libc::PageProtection::PROT_WRITE,
			int flags_ = libc::MapProps::MAP_ANONYMOUS | libc::MapProps::MAP_PRIVATE, int fd_ = -1) {
		prot = (libc::PageProtection)prot_;
		flags = (libc::MapProps)flags_;
		fd = fd_;
		node_size = sizeof(Ptr);
	}

	void *malloc_generic(libc::size_t size);

	void free_generic(void *ptr);

	void *realloc_generic(void *ptr, libc::size_t size);

	template<class T= void>
	void free(T *ptr) {
		free_generic((void *)ptr);
	}

	template<class T= void, class U= void>
	T *realloc(U *ptr, libc::size_t size) {
		return (T*)realloc_generic((void *)ptr, size);
	}

	template<class T= void>
	T *malloc(libc::size_t size) {
		return (T *)malloc_generic(size);
	}
	void destroy();
};
