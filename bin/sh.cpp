extern "C" {
#include <sys/wait.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
}

int main(int argc, char *argv[], char *envp[]) {
	if (argc < 2) {
		while (1) {
			char buff[200] = "";
			write(1, "$ ", 2);
			int readed = read(0, buff, 200);
			buff[readed-1] = 0;

			int argc_ = 0;

			char *argv_[10];
			for (int i = 0; i < 10; i++) {
				argv_[i] = (char*)mmap(0, 30, PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_PRIVATE, -1, 0);
			}

			int len = 6;
			strcat(argv_[0], "build/");
			int i = 0;
			while (buff[i]) {
				if (buff[i] == ' ') {
					argc_++;
					i++;
					len = 0;
					continue;
				}
				argv_[argc_][len] = buff[i];

				len++;
				i++;
			}

			for (int i = argc_+1; i < 10; i++) {
				munmap(argv_[i], 30);
			}
			int status;
			argv_[argc_+1] = 0;

			if (!strcmp(argv_[0], "build/exit")) {
				if (argc_ == 0) return 0;
				return *argv_[1] - 0x30;
			}

			int out = 0;
			int id = fork();
			if (id == 0) {
				out = execve(argv_[0], argv_, envp);
				if (out == -2) {
					printf("command not found %s\n", argv_[0]);
					printf("\e[91m[EXIT: 127]\e[0m\n");
					_exit(127);
				}
				_exit(out);
			} else {
				if (out != 0)
					printf("\e[91m[EXIT: %i]\e[0m\n", out);
				wait(&status);
			}
		}
	}
	return 0;
}
