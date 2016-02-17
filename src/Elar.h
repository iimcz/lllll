/*!
 * @file 		Elar.h
 * @author 		Zdenek Travnicek <travnicek@iim.cz>
 * @date 		27. 11. 2015
 * @copyright	Institute of Intermedia, CTU in Prague, 2013
 * 				Distributed under BSD Licence, details in file doc/LICENSE
 *
 */

#ifndef ELAR_H_
#define ELAR_H_

#include "Light.h"
#include <array>
namespace iim {


class Elar: public Light
{
public:
	Elar(const Json::Value& root);
	virtual ~Elar() noexcept = default;
private:
//	virtual void draw(SDL_Renderer& renderer) const override;
//	virtual void set_from(const uint8_t* start, const uint8_t* end) override;

	virtual void render_points(std::vector<light_source_t>& sources) const override;
	virtual void set_artnet(const uint8_t* start, const uint8_t* end)  override;

	uint8_t mode_;
	uint8_t dimmer_;
	uint8_t flash_;
	float length_;
	float width_ratio_;
	float white_ratio_;

	std::array<color_t, 12> leds_;
};


}






#endif /* ELAR_H_ */
