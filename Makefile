SHELL = /bin/bash
FLAGS = -g -fPIC -Iinclude -z noexecstack -ffreestanding

LIBCXX = stdc++2
CXX := $(wildcard ./src/c++/*.cpp)
FLAGS = -Iinclude -nostdinc++ -nostdlib++

define MAIN_FN
$(filter-out $1,$(MAKECMDGOALS))
endef

define TO_SRC
$(basename $(subst build,src,$1)).c
endef
define TO_OBJ
$(basename $(subst src,build,$1)).o
endef


TESTS := $(foreach test,$(wildcard tests/*.cpp), $(subst .cpp,,$(subst tests/,,$(test))))

bin/%:
	g++ $(FLAGS) $(subst bin,bin-src,$@).c lib/lib$(LIBC).so $(START) -o $@

.ONESHELL:
bins:
	for item in $(wildcard ./bin-src/*.c); do
		make -B $$item
	done

bin-src/%:
	g++ $(FLAGS) $@ lib/lib$(LIBC).so $(START) -o $(subst .c,, $(subst -src,,$@))

run:
	g++ $(FLAGS) $(call MAIN_FN, $@) build/lib$(LIBCXX).so -Wl,-rpath,./build

run-static:
	g++ $(FLAGS) $(call MAIN_FN, $@) -static build/lib$(LIBCXX).a -Wl,-rpath,./build

retest:
	for item in $(TESTS); do
		g++ $(FLAGS) test.cpp tests/$$item.cpp build/lib$(LIBCXX).so -o build/$$item -Wl,-rpath,./build;
	done;

.ONESHELL:
test-all:
	failed=;
	for item in $(TESTS); do
		build/$$item;
		if [[ $$? != 0 ]]; then failed=1; fi;
	done;
	if [[ $$failed == 1 ]]; then echo -e "\e[91mSome Test Failed\e[0m";
	else echo -e "\e[92mAll Test Passed\e[0m";
	fi
	meson test -C build

test:
	meson test -C build

new:
	meson setup build

reconfig:
	meson setup --reconfigure build

compile:
	meson compile -C build

clean:
	rm a.out
	rm *-test.exe

clean-build:
	rm build/*.o

.ONESHELL:
meson-test:
	cd build
	meson test
