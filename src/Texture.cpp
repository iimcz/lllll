/*!
 * @file 		Texture.cpp
 * @author 		Zdenek Travnicek <travnicek@iim.cz>
 * @date 		29. 1. 2016
 * @copyright	Institute of Intermedia, CTU in Prague, 2013
 * 				Distributed under BSD Licence, details in file doc/LICENSE
 *
 */


#include "Texture.h"
#define MAGICKCORE_QUANTUM_DEPTH 16
#define MAGICKCORE_HDRI_ENABLE 0
#include "Magick++.h"
#include "GL/glew.h"
namespace iim {


namespace {
#include "shaders.impl"

unsigned int prepare_texture(Log& log, const std::string& file_name)
{
//	Magick::InitializeMagick(nullptr);
	Magick::Image image;
	Magick::Blob blob;

	try {
		image.read(file_name);
//		log[log_level::info] << "Image read";
		image.write(&blob, "RGB");
//		log[log_level::info] << "Image converted (" << blob.length() << " bytes)";
		GLuint tex;
		glGenTextures(1, &tex);
		GL_CHECK_ERROR
		glActiveTexture(GL_TEXTURE0);
		GL_CHECK_ERROR
		glBindTexture(GL_TEXTURE_2D, tex);
		GL_CHECK_ERROR



		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.columns(), image.rows(), 0, GL_RGB, GL_UNSIGNED_BYTE, blob.data());
		GL_CHECK_ERROR
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		GL_CHECK_ERROR
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		GL_CHECK_ERROR
//		glGenerateMipmap(GL_TEXTURE_2D);
//		GL_CHECK_ERROR
		return tex;

	}
	catch(Magick::Exception& e)
	{
		log[log_level::error] << "Failed to open image " << file_name << ": " << e.what();
	}
	return Texture::invalid_tex_id;

}


}


Texture::Texture(Log& log_):
	log(log_),
	tex_id_{invalid_tex_id},
	vbo_{invalid_tex_id},
	vba_{invalid_tex_id}
{
}

bool Texture::load_image(const std::string& file_name)
{
	if (!file_name.empty()) {
		tex_id_ = prepare_texture(log, file_name);
		if (tex_id_ == invalid_tex_id) {
			log[log_level::error] <<"Texture was not generated";
			return false;
		} else {
			shader_ = std::make_unique<GLProgram>(log);
			shader_->load_shader(GL_VERTEX_SHADER, tex_vs);
			shader_->load_shader(GL_FRAGMENT_SHADER, tex_fs);
			shader_->link();

			std::array<float, 8> coords = {{
					-1.0, -1.0, 1.0, -1.0,
					-1.0, 1.0, 1.0, 1.0
			}};

			glGenBuffers(1, &vbo_);
			glGenVertexArrays(1, &vba_);
			glBindVertexArray(vba_);
			GL_CHECK_ERROR
			glBindBuffer(GL_ARRAY_BUFFER, vbo_);
			GL_CHECK_ERROR

			glBufferData(GL_ARRAY_BUFFER, coords.size() * sizeof(float), &coords[0], GL_STATIC_DRAW);
			GL_CHECK_ERROR


			glEnableVertexAttribArray(0);
			GL_CHECK_ERROR
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, nullptr);
			GL_CHECK_ERROR


		}

	}
	return true;

}

//void Texture::draw(float x0, float y0, float x1, float y1) const
//{
//}
void Texture::draw_full_screen()
{
	if (tex_id_ == invalid_tex_id) return;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_id_);
	shader_->use();
	shader_->set_uniform_int("tex", 0);
	glBindVertexArray(vba_);
	GL_CHECK_ERROR
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	GL_CHECK_ERROR


	glDisable(GL_CULL_FACE);
	GL_CHECK_ERROR
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	GL_CHECK_ERROR

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	GL_CHECK_ERROR



	shader_->stop();
}


}

