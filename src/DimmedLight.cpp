/*!
 * @file 		DimmedLight.cpp
 * @author 		Zdenek Travnicek <travnicek@iim.cz>
 * @date 		26. 1. 2016
 * @copyright	Institute of Intermedia, CTU in Prague, 2013
 * 				Distributed under BSD Licence, details in file doc/LICENSE
 *
 */


#include "DimmedLight.h"

namespace iim {

namespace {

const bool elar_registered = Light::register_light("dimmed", [](const Json::Value& root){
	return  std::make_unique<DimmedLight>(root);
});

}

DimmedLight::DimmedLight(const Json::Value&root)
:Light(root)
	,dimmer_(get_nested_value_or_default(root, 255, "dimmer"))
{

}

void DimmedLight::render_points(std::vector<light_source_t>& sources) const
{
	sources.emplace_back(light_source_t{position_, 0.815, {255, 255, 0, dimmer_}});

}

void DimmedLight::set_artnet(const uint8_t* start, const uint8_t* end)
{
	const auto ch = get_channel();
	const auto count = std::distance(start, end);
	if (count > ch) {
		dimmer_=start[ch];
	}
}
}

