/*!
 * @file 		Window.h
 * @author 		Zdenek Travnicek <travnicek@iim.cz>
 * @date 		12. 11. 2015
 * @copyright	Institute of Intermedia, CTU in Prague, 2013
 * 				Distributed under BSD Licence, details in file doc/LICENSE
 *
 */

#ifndef WINDOW_H_
#define WINDOW_H_
#include "Log.h"
#include "utils.h"
#include "Light.h"
#include "Socket.h"
#include <SDL.h>

namespace iim {



class Window {
public:
	Window(Log& log_, std::vector<std::string> args);
	int run();
private:
	bool process_events();

	Log log;
	managed_resource<SDL_Window*> win_;
	managed_resource<SDL_Renderer*> renderer_;
	managed_resource<SDL_GLContext> ctx_;

	std::vector<Light> lights_;
	using address_t = std::pair<int, int>;
//	// Maps [universe, channel] to [light number, channel]
//	std::map<address_t, address_t> addresses_;

	// Maps universe to a vector of [light_number, channel]
	std::map<int, std::vector<address_t>> universes_;

	int32_t col_count_;
	int32_t row_count_;
	int32_t led_count_;

	int32_t dmx_max_;
	int32_t dmx_offset_;

	bool random_colors_;
	Socket socket_;
};

}



#endif /* WINDOW_H_ */
