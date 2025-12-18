#include <syscall.h>
#include <syscall_enum.h>
#include <file.h>
#include <c-impl.h>

int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("sleep requires 1 argument\n");
		return 1;
	}
	return syscall(atoi(argv[1]), 0, 0, 0, 0, 0, SYS_nanosleep);
}
