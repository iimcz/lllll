/*
 * GLProgram.h
 *
 *  Created on: 10.9.2011
 *      Author: neneko
 */

#ifndef GLPROGRAM_H_
#define GLPROGRAM_H_

#include "GLShader.h"
//#include "Matrix.h"
#include "Log.h"

namespace iim {
typedef std::shared_ptr<class GLProgram> pGLProgram;
class GLProgram {
public:
	GLProgram(Log& log);
	virtual ~GLProgram();
	bool attach_shader(GLShader &shader);
	bool load_shader(GLuint type, std::string source);
	bool load_shader_file(GLuint type, std::string source);
	bool link();
	void use();
	void stop();
	void bind_attrib(GLuint index, const std::string& name);
	void bind_frag_data(GLuint index, const std::string& name);
//	void set_view_matrix(const Matrix4& matrix);
//	void set_model_matrix(const Matrix4& matrix);
//	void set_projection_matrix(const Matrix4 &matrix);
	bool set_uniform_float(std::string name, float value);
	bool set_uniform_int(std::string name, int value);
//	bool set_uniform_vec4(std::string name, const Vector4& value);
	GLuint native();
protected:
	Log	log;
	GLuint program;
	int view_matrix, model_matrix, projection_matrix;
};
}
#endif /* GLPROGRAM_H_ */
