/*!
 * @file 		Light.cpp
 * @author 		Zdenek Travnicek <travnicek@iim.cz>
 * @date 		13. 11. 2015
 * @copyright	Institute of Intermedia, CTU in Prague, 2013
 * 				Distributed under BSD Licence, details in file doc/LICENSE
 *
 */

#include "Light.h"

namespace iim {

void Light::draw(SDL_Renderer& renderer) const
{
	for (const auto& led: leds_) {
		led.draw(renderer);
	}
}

void Light::set_from(uint8_t* start, uint8_t* end)
{
	size_t led = 0;
	while (start < (end-4) && led < leds_.size()) {
		auto& l = leds_[led++];
		l.color().r=*start++;
		l.color().g=*start++;
		l.color().b=*start++;
		l.color().intensity = *start++;
	}


}
}


