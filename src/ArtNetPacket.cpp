/*!
 * @file 		ArtNetPacket.cpp
 * @author 		Zdenek Travnicek <travnicek@iim.cz>
 * @date 		11. 12. 2014
 * @copyright	Institute of Intermedia, CTU in Prague, 2013
 * 				Distributed under BSD Licence, details in file doc/LICENSE
 *
 */

#include "ArtNetPacket.h"
#include <array>

namespace iim {

namespace {
constexpr std::array<uint8_t,header_size> default_artnet_header
	{{'A','r','t','-','N','e','t',0, // Magic
	 0x00, 0x50, 	// Opcode
	 0x00, 14,		// Version
	 0x00,			// Sequence
	 0x00,			// Physical if
	 0x00, 0x00,	// Universe
	 0x00, 0x00		// Length
	}};

constexpr uint16_t sequence_offset = 12;
constexpr uint16_t universe_offset = 14;
constexpr uint16_t length_offset = 16;

void write_into_header_16(std::vector<uint8_t>& header_, uint16_t position, uint16_t value)
{
	header_[position+1]=value&0xFF;
	header_[position]=(value>>8)&0xFF;
}

uint16_t read_from_header_16(const std::vector<uint8_t>& header_, uint16_t position)
{
	return  uint16_t{header_[position+1]} |
			uint16_t{header_[position]}<<8;
}


}

ArtNetPacket::ArtNetPacket(uint16_t universe):data_(default_artnet_header.begin(), default_artnet_header.end())
{
	write_into_header_16(data_, universe_offset, universe);
}


uint8_t& ArtNetPacket::operator[] (uint16_t index)
{
	const uint16_t array_index = index + header_size;
	if (array_index >= data_.size()) {
		if (index > max_values) {
			throw std::out_of_range("Index out of range");
		}
		data_.resize(array_index+1,0);
		write_into_header_16(data_, length_offset, index);
//		data_[header_size-1]=index&0xFF;
//		data_[header_size-2]=(index>>8)&0xFF;
	}
	return data_[array_index];
}
uint8_t ArtNetPacket::operator[] (uint16_t index) const
{
	const uint16_t array_index = index + header_size;
	if (array_index >= data_.size()) throw std::out_of_range("Index out of range");
	return data_[array_index];
}

bool ArtNetPacket::send(Socket& socket)
{
	if (socket.send(data_)) {
		data_[sequence_offset] = (data_[sequence_offset]+1)&0xFF;
		return true;
	}
	return false;
}

bool ArtNetPacket::receive(Socket& socket)
{
	auto v = socket.receive<uint8_t>();


	return false;
}

bool ArtNetPacket::validate_packet(const std::vector<uint8_t>& data)
{
	if (data.size() < header_size) return false;
	if (!std::equal(data.cbegin(), data.cbegin()+8, default_artnet_header.cbegin())) return false;
	const auto size = read_from_header_16(data, length_offset);
	if (data.size() < header_size + size) return false;

	return true;
}
int16_t ArtNetPacket::get_universe(const std::vector<uint8_t>& data)
{
	return read_from_header_16(data, universe_offset);
}

}
