#ifndef TEXTURE_H
#define	TEXTURE_H

#ifdef WIN32
#include <Windows.h>
#endif

#include <string>

#include <GL/glew.h>
#include "imageloader.h"

class Texture
{
public:
	Texture(GLenum TextureTarget, const std::string& FileName);

	bool Load();
	void Bind(GLenum TextureUnit);

private:
	std::string m_fileName;
	GLenum m_textureTarget;
	GLuint m_textureObj;
	Image* m_pImage;
};


#endif
