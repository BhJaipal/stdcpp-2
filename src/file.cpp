#include <file.hpp>
using namespace libc;

extern "C" {
#include <unistd.h>
#include <stdio.h>
#include <string.h>
}

namespace std::io {
	int File::write(const char *msg, libc::size_t n) {
		if (n == INT64_MAX) {
			return ::write(fd, msg, ::strlen(msg));
		} else return ::write(fd, msg, n);
	}
	int File::read(char *msg, libc::size_t n) {
		return ::read(fd, msg, n);
	}

	int File::fwrite(const char *fmt, ...) {
		libc::va_list args;
		va_start(args, fmt);
		char* out;
		::vasprintf(&out, fmt, args);
		va_end(args);
		return write(out);
	}

	void File::close() {
		if (closable) ::close(fd);
		else {
			stderr().write("\e[31mError:\e[0m This file is cannot be closed\b");
		}
	}
}
