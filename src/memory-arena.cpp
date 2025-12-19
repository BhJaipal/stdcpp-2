#include <memory-arena.hpp>
#include <malloc.h>
#include <sys/mman.h>
#define null (pointer_t)0

void MemoryArena::init() {
	auto_defrag_ = true;
	for (size_t i = 0; i < n; i++) {
		head[i].flags = 0;
	}
	inuse_sz = 0;
	total_sz = 0;
}
void pointer_defaults_on(pointer_t ptr) {
	ptr->prev = null;
	ptr->next = NULL;
	ptr->flags = 3;
	ptr->ptr = NULL;
	ptr->size = 0;
}

void MemoryArena::create() {
	init();
	heap_as_default();
}

void MemoryArena::heap_as_default() {
	fd = -1;
	prot = PROT_READ|PROT_WRITE;
	flags = MAP_ANONYMOUS|MAP_PRIVATE;
}

void *MemoryArena::alloc(size_t size) {
	if (head->flags < 3) {
		pointer_defaults_on(head);
		head->size = size;
		head->ptr = mmap(null, size, prot, flags, fd, 0);
		inuse_sz = size;
		total_sz = size;
		return head->ptr;
	}

	pointer_t curr = head;
	size_t i = 0;
	while (curr->next) {
		if (!(curr->size < size || (curr->flags & 1))) {
			break;
		}
		curr = curr->next;
		i++;
	}
	if (!curr->next && (curr->size < size || (curr->flags & 1))) {
		for (unsigned i = 0; i < n; i++) {
			if (!head[i].flags) {
				curr->next = head + i;
				break;
			}
		}
		pointer_defaults_on(curr->next);
		curr->next->size = size;
		curr->next->ptr = mmap((char*)curr->ptr + curr->size, size, prot, flags, fd, 0);
		curr->next->prev = curr;
		inuse_sz += size;
		total_sz += size;
		return curr->next->ptr;
	}
	pointer_t next = curr->next;
	pointer_t new_ = head + i;
	if (curr->size - size) {
		pointer_defaults_on(new_);
		new_->flags = 2;
		new_->size = curr->size - size;

		new_->next = next;
		next->prev = new_;
		curr->next = new_;
	} else {
		new_->flags = false;
	}
	curr->flags = 3;
	curr->size -= size;
	return curr->ptr;
}


void MemoryArena::destroy() {
	munmap(head->ptr, total_sz);
	pointer_t curr = head;
	if (!curr) return;

	while (curr->next) {
		curr = curr->next;
		munmap(curr->prev, sizeof(struct _pointer_t));
	}
	munmap(curr, sizeof(struct _pointer_t));

	for (size_t i = 0; i < n; i++) {
		head[i].flags = 0;
		head[i].size = 0;
		head[i].ptr = 0;
	}
	inuse_sz = 0;
	total_sz = 0;
}

void MemoryArena::dealloc(void *ptr) {
	pointer_t curr = head;
	if (!curr) return;

	while (curr->next && curr->ptr != ptr) {
		curr = curr->next;
	}
	if (curr->ptr != ptr) return;
	curr->flags = 2;

	inuse_sz -= curr->size;

	if (auto_defrag_) auto_defrag();
}

void MemoryArena::auto_defrag() {
	pointer_t curr = head;
	if (!curr) return;

	while (curr->next) {
		while (!(curr->flags & 1) && !(curr->next->flags & 1)) {
			if (!curr->next) break;
			curr->size += curr->next->size;
			pointer_t new_nx = curr->next->next;
			munmap(curr->next, sizeof(struct _pointer_t));
			curr->next = new_nx;
			if (!new_nx) break;
			new_nx->prev = curr;
		}
		curr = curr->next;
	}
}

void MemoryArena::defrag() {}
