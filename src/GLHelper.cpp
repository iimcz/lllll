/*
 * GLHelper.cpp
 *
 *  Created on: Mar 18, 2012
 *      Author: neneko
 */


#include "GLHelper.h"
#include "utils.h"
#include <cassert>
#include <vector>

namespace iim {
namespace GL {

template<>
GLenum gl_type<float>::name() { return GL_FLOAT; }
template<>
GLenum gl_type<double>::name() { return GL_DOUBLE; }

template<>
size_t gl_type<float>::size() { return sizeof(GLfloat); }
template<>
size_t gl_type<double>::size() { return sizeof(GLdouble); }

template<>
void Color3v<float>(float* data)
{
	glColor3fv(data);
}
template<>
void Color3v<double>(double* data)
{
	glColor3dv(data);
}

template <>
void Getv<float>(GLenum pname, float* data)
{
	glGetFloatv(pname,data);
}
template <>
void Getv<double>(GLenum pname, double* data)
{
	glGetDoublev(pname,data);
}

template <>
void MultTransposeMatrix<float>(float* data)
{
	glMultTransposeMatrixf(data);
}
template <>
void MultTransposeMatrix<double>(double* data)
{
	glMultTransposeMatrixd(data);
}

template <>
void LoadMatrix<float>(float* data)
{
	glLoadMatrixf(data);
}
template <>
void LoadMatrix<double>(double* data)
{
	glLoadMatrixd(data);
}

template <>
void UniformMatrix4v<float>(GLint location, GLsizei count,	GLboolean transpose, const float* data)
{
	assert(count==1);
	glUniformMatrix4fv(location, count, transpose, data);
}
template <>
void UniformMatrix4v<double>(GLint location, GLsizei count,	GLboolean transpose, const double* data)
{
	std::vector<float> tmp(16);
	std::copy(data,data+16,tmp.begin());
	UniformMatrix4v(location, count, transpose, &tmp[0]);
}



void init_gl() 
{
//#if !defined(__linux__)
//	glewInit();
	glewExperimental = GL_TRUE;

	GLenum err = glewInit();
	if (err != GLEW_OK) {
		//log << "Failed to initialize GLEW" << endl;
		throw std::runtime_error(std::string("Error while initializing GLEW: ")+std::string(reinterpret_cast<const char *>(glewGetErrorString(err))));
	}
	//log << "GLEW initialized" << endl;
//#endif
}
}
}
