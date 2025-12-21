#include <test.hpp>
#include <string.h>
#include <stdio.h>
#include <memory-arena.hpp>

class AllocTest {
	MemoryArena arena;
	char *name;
	float *x;
	int *a;
public:

	bool name_test() {
		arena.create();
		name = arena.alloc<char>(7);
		strcpy(name, "Jaipal");
		IS_NOT_NULL(name);
		EXPECT(!strcmp(name, "Jaipal"));
		return true;
	}
	bool dealloc_test() {
		a = arena.alloc<int>(4);
		IS_NOT_NULL(a);
		return true;
	}
	bool new_alloc_test() {
		arena.dealloc(name);
		x = arena.alloc<float>(sizeof(float));
		IS_NOT_NULL(x);
		printf("\t\t\e[36mLog:\e[0m %p %p\n", arena.get_head()->ptr, x);
		EXPECT(arena.get_head()->ptr == x);
		return true;
	}
	~AllocTest() {
		arena.dealloc(x);
		arena.dealloc(a);
		arena.destroy();
	}
};


Result enable() {
	Test<AllocTest> test;
	test.add(&AllocTest::name_test);
	test.add(&AllocTest::dealloc_test);
	test.add(&AllocTest::new_alloc_test);
	return test.emit_all();
}
