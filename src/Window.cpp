/*!
 * @file 		Window.cpp
 * @author 		Zdenek Travnicek <travnicek@iim.cz>
 * @date 		12. 11. 2015
 * @copyright	Institute of Intermedia, CTU in Prague, 2013
 * 				Distributed under BSD Licence, details in file doc/LICENSE
 *
 */

#include "Window.h"
#include "SDL_version.h"

namespace iim {

Window::Window(Log& log_):
		log(log_)
{
	SDL_version v;
	SDL_GetVersion(&v);
	log[log_level::info] << "Using SDL version " << int{v.major} << "." << int{v.minor} << "." << int{v.patch};

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE);


	win_ = {SDL_CreateWindow(
				"lllll",
				SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
				800, 600,
				SDL_WINDOW_FULLSCREEN_DESKTOP| SDL_WINDOW_ALLOW_HIGHDPI),
			[](SDL_Window*p){if(p) SDL_DestroyWindow(p);}};
//	ctx_ = {SDL_GL_CreateContext(win_.get()),
//			[](SDL_GLContext p){if(p) SDL_GL_DeleteContext(p);}
//	};
	renderer_ = {SDL_CreateRenderer(
					win_.get(),
					-1,
					SDL_RENDERER_ACCELERATED),
			[](SDL_Renderer*p){if(p) SDL_DestroyRenderer(p);}
	};
	SDL_RendererInfo rinfo;
	SDL_GetRendererInfo(renderer_.get(), &rinfo);
	log[log_level::info] << "Initialized 2D renderer " << rinfo.name << ", " << (rinfo.flags&SDL_RENDERER_ACCELERATED?"with":"without") << " HW acceleration";
}

bool Window::process_events()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:

				return false;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					log[log_level::info] << "Escape pressed";
					return false;
				} else if (event.key.keysym.sym == 'f') {
//					window_fullscreen_ = !window_fullscreen_;
//					SDL_SetWindowFullscreen(window_.get(), window_fullscreen_?SDL_WINDOW_FULLSCREEN_DESKTOP:0);
				} break;
			case SDL_WINDOWEVENT:
				switch (event.window.event) {
					case SDL_WINDOWEVENT_CLOSE:
						log[log_level::info] << "Window closed";
						return false;
				} break;
		}
	}
	return true;
}

int Window::run()
{

	while(process_events())
	{
		SDL_RenderClear(renderer_.get());

		SDL_RenderPresent(renderer_.get());
	}
	return 0;
}



}


