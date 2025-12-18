#include "include/test.hpp"
#include <stdio.h>

Result enable();

int main(int argc, char *argv[]) {
	Result res = enable();

	// enable();
	if (res == Result::PASS) {
		printf("\e[92mTest passed\n\e[0m");
		return 0;
	} else {
		printf("\e[91mTest failed\n\e[0m");
	}
	return 1;
}
