#include "Texture.h"

#include "Image.h"
#include <GLFW/glfw3.h>

Texture::~Texture()
{
	if (mId != 0)
		glDeleteTextures(1, &mId);
}

void
Texture::init()
{
	glGenTextures(1, &mId);
	glBindTexture(GL_TEXTURE_2D, mId);
	
	// Filters and clamping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // GL_NEAREST
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // GL_NEAREST
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);     // GL_CLAMP
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);     // GL_CLAMP
}

void
Texture::bind() const 
{
	glBindTexture(GL_TEXTURE_2D, mId);
}

void
Texture::load(const std::string& name, GLubyte alpha)
{
	if (mId == 0)
	init();
	
	Image image;
	image.load(name);
	
	if (alpha != 255)
	image.setAlpha(alpha);
	
	mWidth = image.width();
	mHeight = image.height();
	
	GLint level = 0;  // Base image level
	GLint border = 0; // No border
	
	glBindTexture(GL_TEXTURE_2D, mId);
	glTexImage2D(GL_TEXTURE_2D,
	             level,
	             GL_RGBA,
	             mWidth,
	             mHeight,
	             border,
	             GL_RGBA,
	             GL_UNSIGNED_BYTE,
	             image.data());
    //glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, int(mWidth), int(mHeight)); // GL_RGBA8, GL_RGB8, GL_RGB16F, GL_RGBA16F

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::loadColorBuffer(const GLsizei width, const GLsizei height, const GLuint swapchain_face) {
	if (mId == 0)
		init();

	mWidth = width;
	mHeight = height;

	GLint level = 0;  // Base image level
	GLint border = 0; // No border

	glBindTexture(GL_TEXTURE_2D, mId);

	GLint readFboId;
	// glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &readFboId);
    //glBindFramebuffer(GL_READ_FRAMEBUFFER, readFboId);
	// HACK: we assume buffer bound is the framebuffer
	{
        glReadBuffer(swapchain_face);
        glCopyTexImage2D(GL_TEXTURE_2D, level, GL_RGBA, 0, 0, mWidth, mHeight, border);
    }
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void
Texture::setWrap(GLuint wp) // GL_REPEAT, GL_CLAMP_TO_EDGE, ...
{
	glBindTexture(GL_TEXTURE_2D, mId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wp);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wp);
	glBindTexture(GL_TEXTURE_2D, 0);
}
