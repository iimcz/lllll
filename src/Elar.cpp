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
#include <limits>
#include <cmath>
namespace iim {

namespace {

const bool elar_registered = Light::register_light("elar", [](const Json::Value& root){
	return  std::make_unique<Elar>(root);
});

const double pi = std::acos(-1.0);

}

Elar::Elar(const Json::Value&root)
:Light(root),
    mode_(get_nested_value_or_default(root, 0, "mode"))
	,dimmer_(255),flash_(0),
	length_(get_nested_value_or_default(root, 1.0, "length")),
	width_ratio_(get_nested_value_or_default(root, 1.5, "ratio")),
	white_ratio_(get_nested_value_or_default(root, 0.5, "white_ratio")),
	orientation_(get_nested_value_or_default(root, 0, "orientation"))
{

}

namespace {
template<typename T, typename T2>
T clamp (T val, T2 minv, T2 maxv) {
	if (val < minv) return minv;
	if (val > maxv) return maxv;
	return val;
}

// Ratio 0 => result = 1, ratio 1 => result = b
template<typename T, typename R>
constexpr T mix(T a, T b, R ratio) {
	return 	  static_cast<T>(clamp(
				a * (R{1.0}-ratio)
				+ b * ratio,
				0.01, 254.8));
}
template<typename T, T max_val = std::numeric_limits<T>::max(), typename R = float>
constexpr T level(T value, T lev) {
	return static_cast<T>(value * static_cast<R>(lev)/static_cast<R>(max_val));
}
}

void Elar::render_points(std::vector<light_source_t>& sources) const
{
	auto pos = position_;
	const uint8_t dimmer = dimmer_;
	const auto len_delta = length_ / leds_.size();
	const auto delta_x = len_delta * std::sin(orientation_ * pi / 180.0);
	const auto delta_y = len_delta * std::cos(orientation_ * pi / 180.0);
	for (const auto&led: leds_) {
		const auto color = color_t{
			level(mix(led.intensity, led.r, white_ratio_), dimmer),
			level(mix(led.intensity, led.g, white_ratio_), dimmer),
			level(mix(led.intensity, led.b, white_ratio_), dimmer),
			255
		};
		sources.emplace_back(light_source_t{pos, len_delta*width_ratio_, len_delta, color});
		pos.y += delta_y;
		pos.x += delta_x;

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
		if (count_total < static_cast<decltype(count_total)>(ch)) {
			return;
		}
		start += ch;
	}

	const auto count = std::distance(start, end);
	set_if_present(mode_, 	mode_channel, start, count);
	set_if_present(dimmer_, dimmer_channel, start, count);
	set_if_present(flash_, 	flash_channel, start, count);

	for (auto i = 0u; i < leds_.size(); ++i) {
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


