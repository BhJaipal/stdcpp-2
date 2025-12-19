#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

#ifndef size_t
typedef long unsigned size_t;
#endif


typedef struct _pointer_t {
	void *ptr;
	size_t size;
	/* enable|in_use */
	char flags;
	_pointer_t *prev, *next;
} *pointer_t;

class MemoryArena {
	int fd;
	int flags;
	int prot;
	/* these are set automatic */
	_pointer_t head[20];
	size_t inuse_sz, total_sz;
	char auto_defrag_;
	size_t n = 20;
public:

	/**
	* @brief It is 1st function to be called to setup memory arena values
	*/
	void init();

	/** @brief uses both init and heap_as_default */
	void create();

	pointer_t get_head() {
		return head;
	}
	/**
	* @brief Set up flags as heap's default flags
	*/
	void heap_as_default();

	/** sets arena.auto_defrag as you wish, default is true, auto defragmentation is done after each memory free */
	void set_auto_defrag(char auto_defrag) {
		auto_defrag_ = auto_defrag;
	}

	/** @brief allocate memory in arena */
	void *alloc(size_t size);

	void dealloc(void *ptr);

	/** WARN: Not implemented yet */
	void defrag();
	void auto_defrag();

	void destroy();
};

#endif // !MEMORY_POOL_H
