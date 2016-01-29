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
#include "ArtNetPacket.h"
#include <random>
#include <fstream>
#include "GLProgram.h"
//#include "GL/gl.h"

namespace iim {

namespace {
#include "shaders.impl"
}


Window::Window(Log& log_, std::vector<std::string> args):
		log(log_)//,random_colors_(false)
	,bg_color_{0, 0, 0, 255},vbo_{0},background_(log),
	initialized_sources_count_(0)
{
	Json::Value root;
	if (args.size() > 1) {
		std::ifstream input(args[1]);
		input >> root;
	} else {
		log[log_level::error] << "No configuration specified!";
		throw std::runtime_error("No configuration specified");
	}
	int width  = get_nested_value_or_default(root, 1920, "window", "size", "x");
	int height = get_nested_value_or_default(root, 1080, "window", "size", "y");
	bool fs    = get_nested_value_or_default(root, false, "window", "fullscreen");
	bg_color_.r = get_nested_value_or_default(root, 0, "window", "background", "r");
	bg_color_.g = get_nested_value_or_default(root, 0, "window", "background", "g");
	bg_color_.b = get_nested_value_or_default(root, 0, "window", "background", "b");

	const std::string bg_filename = get_nested_value_or_default(root, std::string{}, "window", "background", "image");
	std::string address = get_nested_value_or_default(root, "0.0.0.0", "network", "address");
	uint16_t port = get_nested_value_or_default(root, 6454, "network", "port");

	scene_size_.width = get_nested_value_or_default(root, 1.0, "scene", "width");
	scene_size_.height= get_nested_value_or_default(root, 1.0, "scene", "height");
	socket_.bind(address, port);

	SDL_version v;
	SDL_GetVersion(&v);
	log[log_level::info] << "Using SDL version " << int{v.major} << "." << int{v.minor} << "." << int{v.patch};

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE);


	log[log_level::info] << "Creating window " << width <<"x" << height;
	win_ = {SDL_CreateWindow(
				"lllll",
				SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
				width, height,
					(fs?SDL_WINDOW_FULLSCREEN_DESKTOP:0) |
					SDL_WINDOW_ALLOW_HIGHDPI |
					SDL_WINDOW_OPENGL),
			[](SDL_Window*p){if(p) SDL_DestroyWindow(p);}};

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	ctx_ = {SDL_GL_CreateContext(win_.get()),
			[](SDL_GLContext p){if(p) SDL_GL_DeleteContext(p);}
	};
	if (!ctx_.get()) {
		log[log_level::fatal] << "Failed to initialize OpenGL!";
		throw std::runtime_error("Failed to initialize OpenGL");
	}
	{
		auto gl_major = 0;
		auto gl_minor = 0;
		SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &gl_major);
		SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &gl_minor);
		log[log_level::info] << "Initialized OpenGL context, version " << gl_major << "." << gl_minor;
	}
	GL::init_gl();
	glGenBuffers(1, &vbo_);
	auto ggva = glGenVertexArrays;
	if (!ggva) {
		log[log_level::fatal] << "NO glGenVertexArrays!!!!!!!!!";
	}
	glGenVertexArrays(vbas_.size(), &vbas_[0]);

	int w = 0, h = 0;
	SDL_GetWindowSize(win_.get(), &w, &h);
	log[log_level::info] << "Real window size " << w <<"x" << h;

	background_.load_image(bg_filename);

	const auto& json_lights = get_nested_element(root, "lights");
	for (const auto& element: json_lights) {
		auto new_light = Light::generate_light(element);
		auto u = new_light->get_universe();
		if (universes_.size() <= u) {
			universes_.resize(u+1);
		}
		auto& lights_ = universes_.at(u);
		lights_.emplace_back(std::move(new_light));
	}
	const auto light_count = std::accumulate(universes_.cbegin(), universes_.cend(), size_t{0}, [](const auto& a, const auto& b){return a + b.size();});
	const auto universe_count = universes_.size();
	log[log_level::info] << "Generated " << light_count << " lights in " << universe_count << " universe" << (universe_count>1?"s":"");
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
				/*}  else if (event.key.keysym.sym == 'r') {
					random_colors_ = ! random_colors_;*/
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

void Window::render_lights()
{
	sources_.clear();
	for (const auto& universe: universes_) {
		for (const auto& light: universe) {
			light->render_points(sources_);
		}
	}
	if (initialized_sources_count_ < sources_.size()) {
		log[log_level::error] << "Received wrong number of sources...";
		return;
	}

	glClearColor(bg_color_.r / 255.0f, bg_color_.g / 255.0f, bg_color_.b / 255.0f, bg_color_.intensity / 255.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	background_.draw_full_screen();



	glBindVertexArray(vbas_[0]);
	GL_CHECK_ERROR
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	log[log_level::debug] << "Updating buffer with " << sources_.size() << " lights";
	glBufferSubData(GL_ARRAY_BUFFER, 0, sources_.size() * sizeof(light_source_t), &sources_[0]);
	GL_CHECK_ERROR

	shader_->use();
	glDisable(GL_CULL_FACE);
	GL_CHECK_ERROR
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
//		glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFunc(GL_SRC_ALPHA,GL_ZERO);
	glDrawArrays(GL_POINTS, 0, sources_.size());
	GL_CHECK_ERROR

	glDisable(GL_BLEND);
	shader_->stop();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	GL_CHECK_ERROR
	SDL_GL_SwapWindow(win_.get());
}

void Window::prepare_data()
{
	shader_ = std::make_unique<GLProgram>(log);
	shader_->load_shader(GL_VERTEX_SHADER, default_vs);
	shader_->load_shader(GL_FRAGMENT_SHADER, default_fs);
	shader_->load_shader(GL_GEOMETRY_SHADER, default_gs);

	shader_->link();
	shader_->use();

	for (const auto& universe: universes_) {
		for (const auto& light: universe) {
			light->render_points(sources_);
		}
	}
	glBindVertexArray(vbas_[0]);
	GL_CHECK_ERROR
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	GL_CHECK_ERROR

	static_assert(std::is_standard_layout<light_source_t>::value, "Light source has to be standard layout");
	log[log_level::info] << "Initializing buffer for " << sources_.size() << " lights";
	initialized_sources_count_ = sources_.size();
	glBufferData(GL_ARRAY_BUFFER, sources_.size() * sizeof(light_source_t), nullptr, GL_DYNAMIC_DRAW);
	GL_CHECK_ERROR


	glEnableVertexAttribArray(0);
	GL_CHECK_ERROR
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(light_source_t), reinterpret_cast<GLvoid*>(offsetof(light_source_t, position)));
	GL_CHECK_ERROR


	glEnableVertexAttribArray(1);
	GL_CHECK_ERROR
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(light_source_t), reinterpret_cast<GLvoid*>(offsetof(light_source_t, radius_x)));
	GL_CHECK_ERROR


	glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(light_source_t), reinterpret_cast<GLvoid*>(offsetof(light_source_t, color)));
	glEnableVertexAttribArray(2);
//
	if (!shader_->set_uniform_float("scene_width", scene_size_.width)) {
		log[log_level::error] << "Failed to set scene size";
	}
	if (!shader_->set_uniform_float("scene_height", scene_size_.height)) {
		log[log_level::error] << "Failed to set scene size";
	}
}
int Window::run()
{
	std::vector<uint8_t> data;
	data.reserve(530);

	prepare_data();
	while(process_events())
	{
		while (socket_.data_available(15)) {
			log[log_level::verbose] << "Data available!";
			// Reusing the temporary buffer
			data = socket_.receive<uint8_t>(std::move(data));
			log[log_level::verbose] << "Received " << data.size() << " bytes";
			if (!ArtNetPacket::validate_packet(data)) {
				log[log_level::warning] << "Received invalid ARTNet packet";
				continue;
			}
			auto universe = ArtNetPacket::get_universe(data);
			log[log_level::verbose] << "Received data for universe: " << universe;
			if (static_cast<size_t>(universe) < universes_.size() && data.size() > header_size) {
				for (auto& light: universes_[universe]) {
					light->set_artnet(&data.front()+header_size, &data.back());
				}
			}
		}

		render_lights();
	}
	return 0;
}



}



