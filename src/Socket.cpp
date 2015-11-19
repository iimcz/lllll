/*!
 * @file 		Socket.cpp
 * @author 		Zdenek Travnicek <travnicek@iim.cz>
 * @date 		12. 11. 2015
 * @copyright	Institute of Intermedia, CTU in Prague, 2013
 * 				Distributed under BSD Licence, details in file doc/LICENSE
 *
 */

#include "Socket.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <cstring>
#include <poll.h>
namespace iim {

Socket::Socket()
{
	fd_ = {::socket(AF_INET, SOCK_DGRAM | SOCK_NONBLOCK, 0),
		[](int fd){::close(fd);}};

	if (fd_.get() < 0) {
		throw std::runtime_error(std::string{"Failed to create socket: "} + std::strerror(errno));
	}
	int optval = 1;
	if(::setsockopt(fd_.get(), SOL_SOCKET, SO_REUSEADDR,(void *) &optval, sizeof(optval)) <0){
//	 		log[log_level::warning] << "Failed to set SO_REUSEADDR";
	 }
#ifdef SO_REUSEPORT
	 optval = 1;
	 if(::setsockopt(fd_.get(), SOL_SOCKET, SO_REUSEPORT,(void *) &optval, sizeof(optval)) <0){
//	  		log[log_level::warning] << "Failed to set SO_REUSEPORT";
	 }
#endif

	 const addrinfo hints = {AI_PASSIVE, AF_INET, SOCK_DGRAM, 0, 0, nullptr, nullptr, nullptr};
	 managed_resource<addrinfo *> info = {nullptr, [](addrinfo* i){if(i)freeaddrinfo(i);}};
	 const char* addr = nullptr;//"0.0.0.0";
	 	/* int ret = */ ::getaddrinfo(addr,
	 					"6454",
					   &hints,
					   &(info.get()));
	if (::bind(fd_.get(), info.get()->ai_addr, info.get()->ai_addrlen) < 0) {
		throw std::runtime_error(std::string{"Failed to bind socket: "} + std::strerror(errno));
	}


}

bool Socket::data_available(uint32_t timeout_ms)
{
	pollfd fds = {fd_.get(), POLLIN, 0};
	::poll(&fds, 1, timeout_ms);
	return (fds.revents & POLLIN);
}

int Socket::recv_impl()
{
	return ::recvfrom(fd_.get(), &buffer_[0], buffer_.size(), MSG_DONTWAIT, nullptr, nullptr);
}

}



