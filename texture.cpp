#include <iostream>
#include "texture.h"

GLuint loadTexture(Image* image) {		//taken from http://www.videotutorialsrock.com/opengl_tutorial/textures/text.php
	GLuint textureId;
	glGenTextures(1, &textureId); //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
	//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
		0,                            //0 for now
		GL_RGB,                       //Format OpenGL uses for image
		image->width, image->height,  //Width and height
		0,                            //The border of the image
		GL_RGB, //GL_RGB, because pixels are stored in RGB format
		GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
		//as unsigned numbers
		image->pixels);               //The actual pixel data
	return textureId; //Returns the id of the texture
}

Texture::Texture(GLenum TextureTarget, const std::string& FileName) {
	m_textureTarget = TextureTarget;
	m_fileName = FileName;
	m_pImage = NULL;
}

bool Texture::Load() {
	const char * c = m_fileName.c_str();
	m_pImage = loadBMP(c);

	glGenTextures(1, &m_textureObj);
	glBindTexture(m_textureTarget, m_textureObj);
	glTexImage2D(m_textureTarget, 0, GL_RGB, m_pImage->width, m_pImage->height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_pImage->pixels);
	glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	return true;
}

void Texture::Bind(GLenum TextureUnit) {
	glActiveTexture(TextureUnit);
	glBindTexture(m_textureTarget, m_textureObj);
}
