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
#include <SDL.h>
namespace iim {

class Window {
public:
	Window(Log& log_);
	int run();
private:
	bool process_events();

	Log log;
	managed_resource<SDL_Window*> win_;
	managed_resource<SDL_Renderer*> renderer_;
	managed_resource<SDL_GLContext> ctx_;
};

}



#endif /* WINDOW_H_ */
