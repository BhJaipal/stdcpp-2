#pragma once

#include "file.hpp"

namespace std::net {
	class Socket : public std::io::File {
		Socket(int fd_): std::io::File(fd_) {}

	public:
		Socket(int domain, int type, int protocol);
		int bind(libc::uint16_t port);
		int listen(libc::size_t backlog);
		Socket accept();
	};
}
