/*!
 * @file 		Texture.h
 * @author 		Zdenek Travnicek <travnicek@iim.cz>
 * @date 		29. 1. 2016
 * @copyright	Institute of Intermedia, CTU in Prague, 2013
 * 				Distributed under BSD Licence, details in file doc/LICENSE
 *
 */

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "Log.h"
#include "GLProgram.h"
#include <string>
#include <memory>

namespace iim {

class Texture {
public:
	static constexpr unsigned int invalid_tex_id = 0xffffffff;
	Texture(Log& log_);

	bool load_image(const std::string& file_name);

//	void draw(float x0, float y0, float x1, float y1) const ;
	void draw_full_screen();
private:
	Log& log;
	unsigned int tex_id_;
	unsigned int vbo_;
	unsigned int vba_;
	std::unique_ptr<GLProgram> shader_;

};


}



#endif /* TEXTURE_H_ */
