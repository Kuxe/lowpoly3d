#include "framebuffer.hpp"

using namespace gl;

Framebuffer::Framebuffer(const gl::GLsizei width, const gl::GLsizei height, gl::GLsizei samples) {
	
	/** Create and bind framebuffer **/
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	/** Create and bind renderbuffer **/
	glGenRenderbuffers(1, &renderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
	if(samples > static_cast<GLsizei>(GL_MAX_SAMPLES)) {
		printf("WARNING: Number of samples (=%i) for renderbuffer is larger than GL_MAX_SAMPLES, capping samples to GL_MAX_SAMPLES (=%i)\n", samples, static_cast<GLint>(GL_MAX_SAMPLES));
		samples = static_cast<GLsizei>(GL_MAX_SAMPLES);
	}
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_SRGB8_ALPHA8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderbuffer);
	if(glGetError() != GL_NO_ERROR) {
		printf("ERROR: Unknown error in framebuffer constructor during renderbuffer initialization!\n");
	}

	/** Create and bind texture **/
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0); //Bind texture to binded fbo

	if(glGetError() != GL_NO_ERROR) {
		printf("ERROR: Unknown error in framebuffer constructor during texture initialization!\n");
	}

	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(status != GL_FRAMEBUFFER_COMPLETE) {  
		printf("ERROR: Couldn't create framebuffer\n");
	}  
}

Framebuffer::~Framebuffer() {
	glDeleteTextures(1, &texture);
	glDeleteRenderbuffers(1, &renderbuffer);
	glDeleteFramebuffers(1, &fbo);
}

bool Framebuffer::use() {
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	if(glGetError() != GL_NO_ERROR) {
		printf("ERROR: Could not use framebuffer!\n");
		return false;
	}
	return true;
}