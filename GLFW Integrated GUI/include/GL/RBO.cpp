#include "RBO.h"

void RBO::generate(GLuint& id){
	glGenRenderbuffers(1, &id);
}
void RBO::setStorage(const GLuint& id, GLenum internalformat, GLsizei width, GLsizei height) {
	bind(id);
	glRenderbufferStorage(GL_RENDERBUFFER, internalformat, width, height);

}
void RBO::bind(const GLuint& id){
	glBindRenderbuffer(GL_RENDERBUFFER, id);
}
void RBO::unbind(){
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}
void RBO::deleteIt(const GLuint& id){
	glDeleteRenderbuffers(1, &id);
}