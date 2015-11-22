/*!
 * @file 		Socket.h
 * @author 		Zdenek Travnicek <travnicek@iim.cz>
 * @date 		12. 11. 2015
 * @copyright	Institute of Intermedia, CTU in Prague, 2013
 * 				Distributed under BSD Licence, details in file doc/LICENSE
 *
 */

#ifndef SOCKET_H_
#define SOCKET_H_

#include <cstdint>
#include <array>
#include <vector>
#include "utils.h"

namespace iim {

class Socket {
public:
	Socket();

	template<typename T>
	int send(const std::vector<T>& data)
	{
		(void)data;
		return 0;
	}

	template<typename T>
	std::vector<T> receive() {
		return receive(std::vector<T>{});
	}

	template<typename T>
	std::vector<T> receive(std::vector<T> initial)
	{
		auto len = recv_impl();
		auto tlen = len / sizeof(T);
		auto dstart = reinterpret_cast<T*>(&buffer_[0]);
		initial.clear();
		initial.reserve(tlen);

		initial.insert(initial.begin(), dstart, dstart + tlen);
		return std::move(initial);
	}


	bool data_available(uint32_t timeout_ms = 0);
	bool bind(const std::string& address, uint16_t port);
private:
	int recv_impl();

	std::array<uint8_t, 65535> buffer_;
	managed_resource<int> fd_;
};

}



#endif /* SOCKET_H_ */
