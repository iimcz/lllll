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

namespace iim {

class Socket {
public:
	Socket();

	template<typename T>
	int send(const std::vector<T>& data)
	{}

	template<typename T>
	std::vector<T> receive() {
		return receive(std::vector<T>{});
	}

	template<typename T>
	std::vector<T> receive(std::vector<T> initial)
	{

	}

private:
	std::array<uint8_t, 65535> buffer_;
};

}



#endif /* SOCKET_H_ */
