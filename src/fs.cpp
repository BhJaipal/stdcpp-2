#include <fs.hpp>

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

namespace std::fs {
	char *getcwd() {
		return getenv("PWD");
	}
	bool does_file_exist(char *file) {
		int fd = open64(file, O_RDONLY);
		if (fd == -1) {
			return false;
		}
		close(fd);
		return true;
	}
	bool does_directory_exist(char *file) {
		int fd = open64(file, O_RDONLY | O_DIRECTORY);
		if (fd == -1) {
			return false;
		}
		close(fd);
		return true;
	}
}
