#include "FBO.h"

void FBO::generate(GLuint& id) {
	glGenFramebuffers(1, &id);
}

void FBO::attachRBO(GLuint rbo, GLenum attachment){
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, rbo);
}
void FBO::attachRBO(GLuint rbo, GLenum target, GLenum attachment){
	glFramebufferRenderbuffer(target, attachment, GL_RENDERBUFFER, rbo);
}

void FBO::attachTexture(const GLuint& textureId, GLenum attachment){
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, textureId, 0);
}
void FBO::attachTexture(const GLuint& textureId, GLenum target, GLenum attachment){
	glFramebufferTexture2D(target, attachment, GL_TEXTURE_2D, textureId, 0);
}

bool FBO::isComplete() {
	return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}
bool FBO::isComplete(GLenum target) {
	return glCheckFramebufferStatus(target) == GL_FRAMEBUFFER_COMPLETE;
}

void FBO::bind(const GLuint& id) {
	glBindFramebuffer(GL_FRAMEBUFFER, id);//bind buffer
}
void FBO::bind(const GLuint& id, const GLenum target) {
	glBindFramebuffer(target, id);//bind buffer
}

void FBO::unbind() {//also binds default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);//unbind buffer
}
void FBO::unbind(GLenum target) {//also binds default framebuffer
	glBindFramebuffer(target, 0);//unbind buffer
}

void FBO::deleteIt(const GLuint& id) {
	glDeleteFramebuffers(1, &id);
}