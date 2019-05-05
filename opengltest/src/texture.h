#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <string>
#include <GL/glew.h>
#include "stb_image.h"
#include <cassert>

class Texture{
	public:
		Texture(const std::string& fileName);
		virtual ~Texture();

		void Bind(unsigned int unit);

	protected:
	private:
		GLuint m_texture;


};

#endif // TEXTURE_H