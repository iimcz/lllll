/*!
 * @file 		Light.cpp
 * @author 		Zdenek Travnicek <travnicek@iim.cz>
 * @date 		13. 11. 2015
 * @copyright	Institute of Intermedia, CTU in Prague, 2013
 * 				Distributed under BSD Licence, details in file doc/LICENSE
 *
 */

#include "Light.h"
#include <unordered_map>
#include <algorithm>
namespace iim {

namespace {

std::unordered_map<std::string, Light::light_generator> generators;
}
bool Light::register_light(std::string name, light_generator generator)
{
	return generators.emplace(std::move(name), std::move(generator)).second;
}

Light::light_ptr Light::generate_light(const std::string& name, const Json::Value& root)
{
	auto it = generators.find(name);
	if (it == generators.end()) {
		throw std::runtime_error("Generator not found for "+name);
	}
	return it->second(root);
}

Light::light_ptr Light::generate_light(const Json::Value& root)
{
	auto type = get_nested_value_or_default(root, "", "type");
	if (type.empty()) {
		throw std::runtime_error("Light specification is missing type");
	}
	return generate_light(type, root);
}

std::vector<std::string> Light::list_registered()
{
	std::vector<std::string> res;
	std::transform(generators.begin(), generators.end(), std::back_inserter(res),
			[](const auto& a){return a.first;});
	return res;
}
Light::Light(const Json::Value& root)
	:position_{
		static_cast<float>(get_nested_value_or_default(root, 0.0f, "position", "x")),
		static_cast<float>(get_nested_value_or_default(root, 0.0f, "position", "y")),
		static_cast<float>(get_nested_value_or_default(root, 0.0f, "position", "z"))}
	,universe_{static_cast<size_t>(get_nested_value_or_default(root, 0,   "artnet", "universe"))}
	,artnet_channel_{static_cast<size_t>(get_nested_value_or_default(root, 0,   "artnet", "channel"))}
{

}



}


