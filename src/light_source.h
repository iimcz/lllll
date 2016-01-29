/*!
 * @file 		light_source.h
 * @author 		Zdenek Travnicek <travnicek@iim.cz>
 * @date 		26. 1. 2016
 * @copyright	Institute of Intermedia, CTU in Prague, 2013
 * 				Distributed under BSD Licence, details in file doc/LICENSE
 *
 */

#ifndef LIGHT_SOURCE_H_
#define LIGHT_SOURCE_H_

#include "utils.h"

namespace iim {

struct light_source_t {
	position_t position;
	float radius_x;
	float radius_y;
	color_t color;
};


}




#endif /* LIGHT_SOURCE_H_ */
