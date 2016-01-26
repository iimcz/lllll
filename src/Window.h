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
#include "GLHelper.h"
#include <SDL.h>
#include <memory>


namespace iim {



class Window {
public:
	Window(Log& log_, std::vector<std::string> args);
	int run();
private:
	bool process_events();

	Log log;
	managed_resource<SDL_Window*> win_;
	managed_resource<SDL_GLContext> ctx_;


	using lights_t = std::vector<std::unique_ptr<Light>>;
	std::vector<lights_t> universes_;

	Socket socket_;

	color_t bg_color_;

	GLuint vbo_;
	std::array<GLuint, 3> vbas_;
	dimensions_t scene_size_;
};

}



#endif /* WINDOW_H_ */
