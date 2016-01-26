/*
 * GLHelper.h
 *
 *  Created on: Mar 18, 2012
 *      Author: neneko
 */

#ifndef GLHELPER_H_
#define GLHELPER_H_
#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES
#endif
#include "utils.h"
#include <GL/glew.h>
#include <GL/gl.h>
#if defined (__linux__)
#include <GL/glx.h>
#endif
#include <string>

namespace iim {

namespace GL {
typedef std::basic_string<GLchar> glstring;
template <typename T>
struct gl_type {
	static GLenum name();
	static size_t size();
};

template <typename T>
void Color3v(T* data);

template <typename T>
void Getv(GLenum pname, T* data);

template <typename T>
void MultTransposeMatrix(T* data);

template <typename T>
void LoadMatrix(T* data);

template <typename T>
void UniformMatrix4v(GLint location, GLsizei count,	GLboolean transpose, const T* data);

void init_gl();
}
}



#endif /* GLHELPER_H_ */
