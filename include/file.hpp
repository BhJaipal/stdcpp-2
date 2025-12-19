#pragma once

#include <stdint.h>
#include "libc.hpp"

namespace std::io {
	class File {
	protected:
		libc::size_t fd;
		bool closable;
		File() { closable = true; }

	public:
		File& closable_(bool val) {
			closable = val; return *this;
		}

		File(libc::size_t fd_): fd(fd_) { closable = true; }
		int write(const char *msg, libc::size_t n = INT64_MAX);
		int read(char *msg, libc::size_t n = INT64_MAX);
		int fwrite(const char *fmt, ...);
		void close();
	};
	__attribute__((__unused__)) static File stdout() {
		return File(1).closable_(false);
	}
	__attribute__((__unused__)) static File stdin() {
		return File(0).closable_(false);
	}
	__attribute__((__unused__)) static File stderr() {
		return File(2).closable_(false);
	}
}
