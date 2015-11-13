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
#include "json_helpers.h"
#include <random>
#include <fstream>

namespace iim {

Window::Window(Log& log_, std::vector<std::string> args):
		log(log_),col_count_(5),row_count_(17),led_count_(12)
{
	Json::Value root;
	if (args.size() > 1) {
		std::ifstream input(args[1]);
		input >> root;
	}

	col_count_ = get_nested_value_or_default(root, 5, "lights", "columns");
	row_count_ = get_nested_value_or_default(root, 17, "lights", "rows");
	led_count_ = get_nested_value_or_default(root, 12, "lights", "leds");

	SDL_version v;
	SDL_GetVersion(&v);
	log[log_level::info] << "Using SDL version " << int{v.major} << "." << int{v.minor} << "." << int{v.patch};

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE);

	int width = get_nested_value_or_default(root, 1920, "window", "size", "x");
	int height = get_nested_value_or_default(root, 1920, "window", "size", "y");
	bool fs = get_nested_value_or_default(root, false, "window", "fullscreen");

	win_ = {SDL_CreateWindow(
				"lllll",
				SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
				width, height,
					(fs?SDL_WINDOW_FULLSCREEN_DESKTOP:0) |
					SDL_WINDOW_ALLOW_HIGHDPI),
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

	int w = 0, h = 0;
	SDL_GetWindowSize(win_.get(), &w, &h);
	const auto col_size = w / col_count_;
	const auto col_offset = col_size / 2;
	const auto row_size = h / row_count_;

	for (int x = 0; x < col_count_; ++x) {
		for (int i = 0; i < row_count_; ++i) {
			auto pos = position_t {	col_offset + x * col_size,
									i * row_size};
			auto spacing = position_t{	0,
										row_size / led_count_};
			lights_.emplace_back(pos, led_count_, dimensions_t{8, 4}, spacing);
		}
	}
	log[log_level::info] << "Generated " << lights_.size() << " lights";
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

	std::random_device rand;
	std::mt19937 gen(rand());
	std::uniform_int_distribution<> distrib(0, lights_.size() -1 );
	std::uniform_int_distribution<> distrib2(0, led_count_ - 1);
	std::uniform_int_distribution<uint8_t> dist_col(0, 255);

	while(process_events())
	{
		SDL_SetRenderDrawColor( renderer_.get(), 0, 0, 0, 255 );
		SDL_RenderClear(renderer_.get());
		for (const auto& light: lights_) {
			light.draw(*renderer_.get());
		}

		for (auto i = 0; i < 100; ++i) {
			auto idx = distrib(gen);
			auto idx2 = distrib2(gen);
//			log[log_level::info] << "modifying light " << idx << ", light " << idx2;
			auto& col = lights_[idx][idx2];
			col.r = dist_col(gen);
			col.g = dist_col(gen);
			col.b = dist_col(gen);
			col.intensity = dist_col(gen);

		}



		SDL_RenderPresent(renderer_.get());
	}
	return 0;
}



}



