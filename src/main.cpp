/*!
 * @file 		main.cpp
 * @author 		Zdenek Travnicek <travnicek@iim.cz>
 * @date 		12. 11. 2015
 * @copyright	Institute of Intermedia, CTU in Prague, 2015
 * 				Distributed under BSD Licence, details in file doc/LICENSE
 *
 */

#include "Window.h"
#include <iostream>
int main(int argc, char** argv)
{
	std::vector<std::string> args(argv, argv + argc);
	iim::Log log{std::clog};
	log[iim::log_level::info] << "lllll Visualization starting ...";
	{
		auto res = iim::Light::list_registered();
		auto ll = log[iim::log_level::info];
		ll << "Registered light types: ";
		for (const auto& l: res) {
			ll << l << ", ";
		}
	}
	iim::Window win{log, args};
	win.run();
	log[iim::log_level::info] << "lllll Visualization shutting down ...";
}
