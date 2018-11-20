#include "framebuffer.hpp"

using namespace gl;

namespace lowpoly3d {

Framebuffer::Framebuffer(const std::string& framebufferName, const gl::GLsizei width, const gl::GLsizei height, gl::GLsizei samples) :
	framebufferName(framebufferName), samples(samples), multisampled(samples > 0) {
	/** Create and bind texture **/
	const GLenum textureTarget = multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &texture);
	glBindTexture(textureTarget, texture);
	if(multisampled) {
		glTexImage2DMultisample(textureTarget, samples, GL_RGB, width, height, GL_TRUE);
	} else {
		glTexImage2D(textureTarget, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(textureTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(textureTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
	glBindTexture(textureTarget, 0);
	if(glGetError() != GL_NO_ERROR) {
		printf("ERROR: Unknown error in framebuffer %s constructor during texture initialization!\n", framebufferName.c_str());
	}


	/** Create and bind renderbuffer **/
	glGenRenderbuffers(1, &renderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
	if(samples > static_cast<GLsizei>(GL_MAX_SAMPLES)) {
		printf("WARNING: Number of samples (=%i) for renderbuffer in framebuffer %s is larger than GL_MAX_SAMPLES, capping samples to GL_MAX_SAMPLES (=%i)\n", samples, framebufferName.c_str(), static_cast<GLint>(GL_MAX_SAMPLES));
		samples = static_cast<GLsizei>(GL_MAX_SAMPLES);
	}

	if(multisampled) {
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH_COMPONENT24, width, height);
	} else {
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
	}
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	if(glGetError() != GL_NO_ERROR) {
		printf("ERROR: Unknown error in framebuffer %s constructor during renderbuffer initialization!\n", framebufferName.c_str());
	}

	/** Create and bind framebuffer **/
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureTarget, texture, 0); //Bind texture to binded fbo
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);
	ok();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	if(glGetError() != GL_NO_ERROR) {
		printf("ERROR: Unknown error in framebuffer  %s constructor during texture initialization!\n", framebufferName.c_str());
	}
}

Framebuffer::~Framebuffer() {
	glDeleteTextures(1, &texture);
	glDeleteRenderbuffers(1, &renderbuffer);
	glDeleteFramebuffers(1, &fbo);
}

bool Framebuffer::use(const gl::GLenum target) const {
	glBindFramebuffer(target, fbo);
	if(glGetError() != GL_NO_ERROR) {
		printf("ERROR: Could not use framebuffer %s!\n", framebufferName.c_str());
		return false;
	}
	return true;
}

bool Framebuffer::ok() const {
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(status != GL_FRAMEBUFFER_COMPLETE) {
		printf("ERROR: Couldn't create framebuffer %s, got ", framebufferName.c_str());
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
				printf("GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE (images have different number of multisamples, which is illegal)\n");
				break;
			} case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS: {
				printf("GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS (if a layered image is attached to one attachment, then all attachments must be layered attachments\n");
				break;
			}
		}
		return false;
	}
	return true;
}

void Framebuffer::notified(const OnResize& evt) {
	const GLenum textureTarget = samples > 0 ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
	glBindTexture(textureTarget, texture);
	glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
	if(multisampled) {
		glTexImage2DMultisample(textureTarget, samples, GL_RGB, evt.width, evt.height, GL_TRUE);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH_COMPONENT24, evt.width, evt.height);
	} else {
		glTexImage2D(textureTarget, 0, GL_RGB, evt.width, evt.height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, evt.width, evt.height);
	}
	if(glGetError() != GL_NO_ERROR) {
		printf("WARNING: Could not resize framebuffer %s!\n", framebufferName.c_str());
	}
}

GLuint Framebuffer::getTexture() const {
	return texture;
}

}
