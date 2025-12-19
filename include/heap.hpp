#pragma once
#include "libc.hpp"

namespace libc {
	extern "C" {
#include <sys/mman.h>
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
	int prot;
	int flags;
	int fd;

public:
	Heap(int prot_ = PROT_READ | PROT_WRITE,
			int flags_ = MAP_ANONYMOUS | MAP_PRIVATE, int fd_ = -1) {
		prot = prot_;
		flags = flags_;
		fd = fd_;
		head = (Ptr *)NULL;
		end = (Ptr *)NULL;
		node_size = sizeof(Ptr);
	}

	void *malloc_generic(libc::size_t size);

	void free_generic(void *ptr);
	Ptr* get_head() { return head; }

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
