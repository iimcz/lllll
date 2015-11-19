/*!
 * @file 		Light.h
 * @author 		Zdenek Travnicek <travnicek@iim.cz>
 * @date 		13. 11. 2015
 * @copyright	Institute of Intermedia, CTU in Prague, 2013
 * 				Distributed under BSD Licence, details in file doc/LICENSE
 *
 */

#ifndef LIGHT_H_
#define LIGHT_H_

#include "LED.h"
#include <vector>
namespace iim {

class Light {
public:
	Light(position_t position, size_t count, dimensions_t dim, position_t spacing)
	//:position_(position), spacing_(spacing)
	{
		leds_.reserve(count);
		for (size_t i = 0; i < count; ++i) {
			leds_.emplace_back(dim, position);
			position += spacing;
		}

	}
	~Light() noexcept = default;

	void draw(SDL_Renderer& renderer) const;

	color_t& operator[](size_t index) {
		return leds_.at(index).color();
	}

	void set_from(uint8_t* start, uint8_t* end);
private:
//	position_t position_;
	std::vector<Led> leds_;
//	position_t spacing_;

};


}



#endif /* LIGHT_H_ */
