/*
 * GLProgram.cpp
 *
 *  Created on: 10.9.2011
 *      Author: neneko
 */

#include "GLProgram.h"
#include <iostream>
namespace iim {
GLProgram::GLProgram(Log& log_):log(log_)
{
//	log.set_label("[GL Program]");
	program = glCreateProgram();
}

GLProgram::~GLProgram()
{

}

bool GLProgram::attach_shader(GLShader &shader)
{
	glAttachShader(program,shader.get_shader());
	return true;
}

bool GLProgram::link()
{
	log[log_level::info] << "Linking program";
	glLinkProgram(program);
	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (linked) {
		view_matrix = glGetUniformLocation(program,"view_matrix");
		model_matrix = glGetUniformLocation(program,"model_matrix");
		projection_matrix  = glGetUniformLocation(program,"projection_matrix");
		log[log_level::info] << "vm: " << view_matrix << ", mm: " << model_matrix << ", pm: " << projection_matrix;
		return true;
	}
	GLint blen = 0;
	GLsizei slen = 0;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH , &blen);
	if (blen > 1)
	{
	 GL::glstring compiler_log(blen,0);
	 glGetInfoLogARB(program, blen, &slen, &compiler_log[0]);
	 log[log_level::info] << "compiler_log:" <<  compiler_log;
	 //delete compiler_log;

	}
	return false;
}

void GLProgram::use()
{
	glUseProgram(program);
}
void GLProgram::stop()
{
	glUseProgram(0);
}

bool GLProgram::load_shader_file(GLuint type, std::string source)
{
	GLShader shader(log, type);
	log[log_level::info] <<"Loading " << source;
	if (!shader.load_file(source)) {
		log[log_level::error] << "Failed to load shader " << source;
		return false;
	}
	log[log_level::info] << "Compiling " << source;
	if (!shader.compile()) {
		log[log_level::info] << "Failed to compile shader "<< source;
	    return false;
	}
	return attach_shader(shader);
}
bool GLProgram::load_shader(GLuint type, std::string source)
{
	GLShader shader(log, type);
	log[log_level::info] <<"Loading shader";
	if (!shader.load(source)) {
		log[log_level::error] << "Failed to load shader";
		return false;
	}
	log[log_level::info] <<"Compiling ";
	if (!shader.compile()) {
		log[log_level::error] << "Failed to compile shader";
	    return false;
	}
	return attach_shader(shader);
}

void GLProgram::bind_attrib(GLuint index, const std::string& name)
{
	glBindAttribLocation(program,index,name.c_str());
}
void GLProgram::bind_frag_data(GLuint index, const std::string& name)
{
	glBindFragDataLocation(program, index, name.c_str());
}
//void GLProgram::set_view_matrix(const Matrix4& matrix)
//{
//	GL::UniformMatrix4v(view_matrix, 1, GL_TRUE, &matrix.data()[0]);
//}
//void GLProgram::set_model_matrix(const Matrix4 &matrix)
//{
//	GL::UniformMatrix4v(model_matrix, 1, GL_TRUE, &matrix.data()[0]);
//}
//void GLProgram::set_projection_matrix(const Matrix4 &matrix)
//{
//	GL::UniformMatrix4v(projection_matrix, 1, GL_TRUE, &matrix.data()[0]);
//}
GLuint GLProgram::native()
{
	return program;
}
bool GLProgram::set_uniform_float(std::string name, float value)
{
	glGetError();
	GLint loc = glGetUniformLocation(program,name.c_str());
	if (loc <0) return false;
	glUniform1f(loc,value);
	if (glGetError()) return false;
	return true;
}
//bool GLProgram::set_uniform_vec4(std::string name, const Vector4& value)
//{
//	glGetError();
//	GLint loc = glGetUniformLocation(program,name.c_str());
//	if (loc <0) return false;
//	glUniform4f(loc,value[0],value[1],value[2],value[3]);
//	if (glGetError()) return false;
//	return true;
//}

}
