/*!
 * @file 		LED.cpp
 * @author 		Zdenek Travnicek <travnicek@iim.cz>
 * @date 		13. 11. 2015
 * @copyright	Institute of Intermedia, CTU in Prague, 2013
 * 				Distributed under BSD Licence, details in file doc/LICENSE
 *
 */

#include "LED.h"
#include "sdl_utils.h"
namespace iim {

namespace {
void set_color(SDL_Renderer& renderer, const color_t& col)
{
	uint8_t r = col.r * (col.intensity * 1.0 / 255.0);
	uint8_t g = col.g * (col.intensity * 1.0 / 255.0);
	uint8_t b = col.b * (col.intensity * 1.0 / 255.0);

	SDL_SetRenderDrawColor( &renderer, r, g, b, SDL_ALPHA_OPAQUE);
}
}

void Led::draw(SDL_Renderer& renderer) const
{
	set_color(renderer, color_);
	const auto r = make_rect(position_, size_);

	SDL_RenderFillRect( &renderer, &r );
}


}


