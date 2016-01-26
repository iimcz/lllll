/*!
 * @file 		Light.h
 * @author 		Zdenek Travnicek <travnicek@iim.cz>
 * @date 		13. 11. 2015
 * @copyright	Institute of Intermedia, CTU in Prague, 2013
 * 				Distributed under BSD Licence, details in file doc/LICENSE
 *
 */

#ifndef LIGHT_H_
#define LIGHT_H_

#include "utils.h"
#include "json_helpers.h"
#include "light_source.h"
#include <vector>
#include <functional>
#include <memory>


namespace iim {

class Light {
public:
	Light(const Json::Value& root);
	virtual ~Light() noexcept = default;

	virtual void render_points(std::vector<light_source_t>& sources) const = 0;
	virtual void set_artnet(const uint8_t* start, const uint8_t* end) = 0;

	virtual size_t get_universe() const { return universe_; }
	virtual size_t get_channel() const { return artnet_channel_; }

	using light_ptr = std::unique_ptr<Light>;
	using light_generator = std::function<light_ptr(const Json::Value&)>;

	static bool register_light(std::string name, light_generator generator);
	static light_ptr generate_light(const std::string& name, const Json::Value& root);
	static light_ptr generate_light(const Json::Value& root);

	static std::vector<std::string> list_registered();
protected:
	position_t position_;

private:
	size_t universe_;
	size_t artnet_channel_;


};


}



#endif /* LIGHT_H_ */
