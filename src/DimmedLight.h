/*!
 * @file 		DimmedLight.h
 * @author 		Zdenek Travnicek <travnicek@iim.cz>
 * @date 		26. 1. 2016
 * @copyright	Institute of Intermedia, CTU in Prague, 2013
 * 				Distributed under BSD Licence, details in file doc/LICENSE
 *
 */

#ifndef DIMMEDLIGHT_H_
#define DIMMEDLIGHT_H_

#include "Light.h"
namespace iim {

class DimmedLight: public Light
{
public:
	DimmedLight(const Json::Value& root);
	virtual ~DimmedLight() noexcept = default;
private:
	virtual void render_points(std::vector<light_source_t>& sources) const override;
	virtual void set_artnet(const uint8_t* start, const uint8_t* end)  override;

	uint8_t dimmer_;
};


}






#endif /* DIMMEDLIGHT_H_ */
