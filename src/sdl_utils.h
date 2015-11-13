/*!
 * @file 		sdl_utils.h
 * @author 		Zdenek Travnicek <travnicek@iim.cz>
 * @date 		13. 11. 2015
 * @copyright	Institute of Intermedia, CTU in Prague, 2013
 * 				Distributed under BSD Licence, details in file doc/LICENSE
 *
 */

#ifndef SDL_UTILS_H_
#define SDL_UTILS_H_

#include "utils.h"
#include "SDL.h"

namespace iim {

inline SDL_Rect make_rect(const position_t& pos, const dimensions_t& dim)
{
	return {pos.x, pos.y,
			dim.width, dim.height
			};
}

}


#endif /* SDL_UTILS_H_ */
