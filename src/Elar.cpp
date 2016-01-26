/*!
 * @file 		Elar.cpp
 * @author 		Zdenek Travnicek <travnicek@iim.cz>
 * @date 		27. 11. 2015
 * @copyright	Institute of Intermedia, CTU in Prague, 2013
 * 				Distributed under BSD Licence, details in file doc/LICENSE
 *
 */

#include "Elar.h"
#include "json_helpers.h"
namespace iim {

namespace {

const bool elar_registered = Light::register_light("elar", [](const Json::Value& root){
	return  std::make_unique<Elar>(root);
});

}

Elar::Elar(const Json::Value&root)
:Light(root),
    mode_(get_nested_value_or_default(root, 0, "mode"))
	,dimmer_(255),flash_(0)
{

}

void Elar::render_points(std::vector<light_source_t>& sources) const
{
	auto pos = position_;
	for (const auto&led: leds_) {
		// TODO: This is just a placeholder to get some data
		sources.emplace_back(light_source_t{pos, 0.04, {255, 255, 255, dimmer_}});
		pos.y+=0.1;

	}
}

namespace {

bool set_if_present(uint8_t& var, uint16_t offset, const uint8_t* start, const size_t count)
{
	if (count < offset) {
		return false;
	}
	var = start[offset];
	return true;
}

constexpr uint16_t mode_channel = 0;
constexpr uint16_t dimmer_channel = 1;
constexpr uint16_t flash_channel = 2;
constexpr uint16_t led_start_channel = 3;

}


void Elar::set_artnet(const uint8_t* start, const uint8_t* end)
{
	{
		const auto count_total = std::distance(start, end);
		const auto& ch = get_channel();
		if (count_total < ch) {
			return;
		}
		start += ch;
	}

	const auto count = std::distance(start, end);
	set_if_present(mode_, 	mode_channel, start, count);
	set_if_present(dimmer_, dimmer_channel, start, count);
	set_if_present(flash_, 	flash_channel, start, count);

	for (int i = 0u; i < leds_.size(); ++i) {
		auto& led = leds_[i];
//		auto& col = led.color();
		const auto channel = i * 4 + led_start_channel;
		if (!set_if_present(led.r, channel + 0, start, count) ||
			!set_if_present(led.g, channel + 1, start, count) ||
			!set_if_present(led.b, channel + 2, start, count) ||
			!set_if_present(led.intensity, channel + 3, start, count)) {
			break;
		}
	}

}


}


