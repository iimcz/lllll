/*!
 * @file 		LED.h
 * @author 		Zdenek Travnicek <travnicek@iim.cz>
 * @date 		13. 11. 2015
 * @copyright	Institute of Intermedia, CTU in Prague, 2013
 * 				Distributed under BSD Licence, details in file doc/LICENSE
 *
 */

#ifndef LED_H_
#define LED_H_
#include "utils.h"
#include "SDL.h"
namespace iim {

class Led
{
public:
	Led(dimensions_t size, position_t position):size_(size),
			color_{0, 0, 255, 255}, position_(position) {}
	~Led() noexcept = default;
	void draw(SDL_Renderer& renderer) const;
	color_t& color() { return color_; }
	const color_t& color() const { return color_; }
private:
	dimensions_t size_;
	color_t color_;
	position_t position_;

};

}



#endif /* LED_H_ */
