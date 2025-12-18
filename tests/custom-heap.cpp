#include <test.hpp>
#include <malloc.h>
#include <string.h>
#include <sys/mman.h>
#include <heap.hpp>

class CustomHeap {
	Heap heap;
	char *name;
public:

	CustomHeap() {
		name = heap.malloc<char>(5);
		strcpy(name, "Hema");
		name = heap.realloc<char>(name, 9);
		strcat(name, " bha");
	}
	bool equal() {
		EXPECT(heap.get_head()->ptr == name);
		heap.free(name);
		return true;
	}
	~CustomHeap() {
		heap.destroy();
	}
};
void *__gxx_personality_v0 = NULL;
Result enable() {
	Test<CustomHeap> test;
	test.add(&CustomHeap::equal);
	test.add([]() {
		EXPECT(5 > 2);
		return true;
	});
	int y = 8;
	test.add([y]() {
		EXPECT(y > 11);
		return true;
	});
	return test.emit_all();
}
