/*
 * GLShader.h
 *
 *  Created on: 10.9.2011
 *      Author: neneko
 */

#ifndef GLSHADER_H_
#define GLSHADER_H_
#include "GLHelper.h"
#include "Log.h"
namespace iim {

class GLShader {
public:
	GLShader(Log& log, GLenum type);
	virtual ~GLShader();
	bool load_file(std::string filename);
	bool load(std::string text);
	bool compile();
	GLuint get_shader();
private:
	Log& log;
	GLenum type;
	GL::glstring shader_text;
	GLint shader_size;
	GLuint shader_object;
};
}
#endif /* GLSHADER_H_ */
