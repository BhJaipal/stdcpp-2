#include <libc.hpp>
#include <socket.hpp>

extern "C" {
namespace libc::net {
#include <netinet/in.h>
}

#include <sys/syscall.h>
}
using namespace libc::net;

namespace std::net {
	int Socket::bind(libc::uint16_t port) {
		libc::net::sockaddr_in addr;
		addr.sin_family = PF_INET;
		addr.sin_port = htons(port);
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
		return libc::syscall(SYS_bind, fd, (libc::size_t)&addr, sizeof(addr));
	}

	Socket::Socket(int domain, int type, int protocol) {
		fd = libc::syscall(SYS_socket, domain, type, protocol);
		if (fd == ULONG_MAX) {
			io::stdout().write("Failed to connect socket\n");
			libc::syscall(SYS_exit, 1);
		}
	}
	int Socket::listen(libc::size_t backlog) {
		return libc::syscall(SYS_listen, fd, backlog);
	}
	Socket Socket::accept() {
		libc::net::sockaddr_in client_Addr;
		libc::socklen_t client_len = sizeof(client_Addr);
		int client = libc::syscall(SYS_accept, fd, (libc::size_t)&client_Addr, (libc::size_t)&client_len);
		return Socket(client);
	}
}
