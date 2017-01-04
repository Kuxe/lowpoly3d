#include "framebuffer.hpp"

using namespace gl;

Framebuffer::Framebuffer(const gl::GLsizei width, const gl::GLsizei height, gl::GLsizei samples) {
	/** Create and bind texture **/
	//TODO: Replace all GL_TEXTURE_2D with GL_TEXTURE_2D_MULTISAMPLE
	const GLenum textureTarget = GL_TEXTURE_2D;
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &texture);
	glBindTexture(textureTarget, texture);
	glTexImage2D(textureTarget, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0); //TODO: Dont forget to replace this one (when multisampling error goes away with 0 => GL_TRUE but then screen is black)
	glTexParameteri(textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(textureTarget, 0);


	/** Create and bind renderbuffer **/
	glGenRenderbuffers(1, &renderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
	if(samples > static_cast<GLsizei>(GL_MAX_SAMPLES)) {
		printf("WARNING: Number of samples (=%i) for renderbuffer is larger than GL_MAX_SAMPLES, capping samples to GL_MAX_SAMPLES (=%i)\n", samples, static_cast<GLint>(GL_MAX_SAMPLES));
		samples = static_cast<GLsizei>(GL_MAX_SAMPLES);
	}
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height); //TODO: This should be multisample too
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	if(glGetError() != GL_NO_ERROR) {
		printf("ERROR: Unknown error in framebuffer constructor during renderbuffer initialization!\n");
	}

	/** Create and bind framebuffer **/
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureTarget, texture, 0); //Bind texture to binded fbo
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(status != GL_FRAMEBUFFER_COMPLETE) {
		printf("ERROR: Couldn't create framebuffer, got ");
		switch(status) {
			default: {
				printf("Unknown reason\n");
			} case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: {
				printf("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT (lookup \"attachment completeness\")\n");
				break;
			} case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: {
				printf("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT (framebuffer width or height is zero)\n");
				break;
			} case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: {
				printf("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER (each drawbuffer must specify color attachment with attached image)\n");
				break;
			} case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER: {
				printf("GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER (read buffer must specify attachent with attached image)\n");
				break;
			} case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE: {
				printf("GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE (images have different number of multisamples, which is illegal\n");
				break;
			} case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS: {
				printf("GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS (if a layered image is attached to one attachment, then all attachments must be layered attachments\n");
				break;
			}
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	if(glGetError() != GL_NO_ERROR) {
		printf("ERROR: Unknown error in framebuffer constructor during texture initialization!\n");
	}
}

Framebuffer::~Framebuffer() {
	glDeleteTextures(1, &texture);
	glDeleteRenderbuffers(1, &renderbuffer);
	glDeleteFramebuffers(1, &fbo);
}

bool Framebuffer::use() const {
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	if(glGetError() != GL_NO_ERROR) {
		printf("ERROR: Could not use framebuffer!\n");
		return false;
	}
	return true;
}

void Framebuffer::notify(const OnResize& evt) {
	const GLenum textureTarget = GL_TEXTURE_2D;
	const GLsizei width = evt.width, height = evt.height;
	glBindTexture(textureTarget, texture);
	glTexImage2D(textureTarget, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0); //TODO: Dont forget to replace this one (when multisampling error goes away with 0 => GL_TRUE but then screen is black)
	glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height); //TODO: This should be multisample too
	if(glGetError() != GL_NO_ERROR) {
		printf("WARNING: Could not resize framebuffer!\n");
	}
}

GLuint Framebuffer::getTexture() const {
	return texture;
}
