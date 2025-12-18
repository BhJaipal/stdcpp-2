#include <heap.hpp>
namespace libc {
	extern "C" {
#include <string.h>
	}
}

void* Heap::malloc_generic(libc::size_t size) {
	void *ptr = libc::mmap(0, size, prot, flags, fd, 0);
	if (!head) {
		head = (Ptr*)libc::mmap(NULL, node_size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
		head->ptr = ptr;
		head->size = size;
		head->next = (Ptr*)NULL;
		end = head;
		return ptr;
	}
	Ptr *next = (Ptr*)libc::mmap(NULL, node_size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	next->ptr = ptr;
	next->size = size;
	next->next = (Ptr*)NULL;

	end->next = next;
	end = next;
	return ptr;
}

void Heap::free_generic(void *ptr) {
	Ptr *head = this->head;
	if (!head) return;
	Ptr *prev = (Ptr*)NULL;

	while (head->ptr != ptr) {
		prev = head;
		head = head->next;
	}
	if (prev)
		prev->next = head->next;
	libc::munmap(head->ptr, head->size);
	libc::munmap(head, node_size);
	if (prev)
		prev->next = (Ptr*)NULL;
	else {
		this->head = (Ptr*)NULL;
	}
}

void *Heap::realloc_generic(void *ptr, libc::size_t size) {
	void *new_ptr = libc::mmap(0, size, prot,
				  flags, fd, 0);

	Ptr *head = this->head;
	if (!head) return NULL;
	libc::size_t len = 0;
	while (head && head->ptr != ptr) {
		head = head->next;
	}
	len = head->size;
	head->ptr = new_ptr;
	head->size = size;
	libc::memcpy(new_ptr, ptr, len);
	libc::munmap(ptr, len);
	return new_ptr;
}
void Heap::destroy() {
	Heap *heap = this;
	Ptr *head = heap->head;
	Ptr *prev = (Ptr*)NULL;

	while (head) {
		prev = head;
		head = head->next;
		libc::munmap(prev->ptr, prev->size);
		libc::munmap(prev, heap->node_size);
	}
}
