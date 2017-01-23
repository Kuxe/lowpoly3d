#include "depthframebuffer.hpp"

using namespace gl;

namespace lowpoly3d {

DepthFramebuffer::DepthFramebuffer(const gl::GLsizei width, const gl::GLsizei height) {

	/** Create and bind framebuffer **/
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	/** Create and bind texture **/
	const GLenum textureTarget = GL_TEXTURE_2D;
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &texture);
	glBindTexture(textureTarget, texture);
	glTexImage2D(textureTarget, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(textureTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(textureTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	float outOfTextureColor[] = {1.0f, 0.0f, 1.0f, 1.0f};
	glTexParameterfv(textureTarget, GL_TEXTURE_BORDER_COLOR, outOfTextureColor);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, textureTarget, texture, 0); //Bind texture to binded fbo
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(status != GL_FRAMEBUFFER_COMPLETE) {
		printf("ERROR: Couldn't create depth framebuffer, got ");
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
		printf("ERROR: Unknown error in depth framebuffer constructor during texture initialization!\n");
	}
}

DepthFramebuffer::~DepthFramebuffer() {
	glDeleteTextures(1, &texture);
	glDeleteFramebuffers(1, &fbo);
}

bool DepthFramebuffer::use() const {
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	if(glGetError() != GL_NO_ERROR) {
		printf("ERROR: Could not use depth framebuffer!\n");
		return false;
	}
	return true;
}

GLuint DepthFramebuffer::getTexture() const {
	return texture;
}

}
