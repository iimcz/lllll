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
}


