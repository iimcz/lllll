/*
 * GLShader.cpp
 *
 *  Created on: 10.9.2011
 *      Author: neneko
 */

#include "GLShader.h"
#include <fstream>
#include <iostream>
#include <GL/glext.h>
//#include <cstring>

namespace iim {

GLShader::GLShader(Log& log_, GLenum type):log(log_),type(type)
{
//	log.set_label("[GL Shader]");
}

GLShader::~GLShader()
{

}

bool GLShader::load_file(std::string filename)
{
	std::ifstream file;
	file.open(filename,std::ios::binary);
	if (file.bad()) return false;
	file.seekg(0,std::ios::end);
	shader_size = file.tellg();
	file.seekg(0,std::ios::beg);
	if (!shader_size) return false;
	shader_text.resize(shader_size);
	file.read(&shader_text[0], shader_size);
	return true;
}
bool GLShader::load(std::string text)
{
	shader_size = text.size();
	if (!shader_size) return false;
	//if (shader_text) delete [] shader_text;
	//shader_text = new GLchar[++shader_size+1];

	//memset(shader_text,0,shader_size+1);
	//memcpy(shader_text,text.c_str(),shader_size-1);
	//std::cerr << "Allocating " << shader_size << endl;
	shader_text.resize(++shader_size+1);
	std::copy(text.begin(),text.end(),shader_text.begin());
	shader_text[shader_size] = 0;
	return true;
}
bool GLShader::compile()
{
	shader_object = glCreateShader(type);
	const GLchar *source = shader_text.c_str();
	glShaderSource(shader_object,1,reinterpret_cast<const GLchar**>(&source),&shader_size);
	shader_text.resize(shader_size);
	glCompileShader(shader_object);
	GLint compiled;
	glGetObjectParameterivARB(shader_object, GL_COMPILE_STATUS, &compiled);
//	if (compiled) return true;
	GLint blen = 0;
	GLsizei slen = 0;
	glGetShaderiv(shader_object, GL_INFO_LOG_LENGTH , &blen);
	if (blen > 1)
	{
	 GL::glstring compiler_log(blen,0);
	 glGetInfoLogARB(shader_object, blen, &slen, &compiler_log[0]);
	 std::cout << "compiler_log:" <<  compiler_log <<std::endl;
	}
	return compiled;
}

GLuint GLShader::get_shader()
{
	return shader_object;
}
}
