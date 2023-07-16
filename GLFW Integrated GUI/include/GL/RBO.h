#pragma once

#include <glad/glad.h>

class RBO {
public:
	RBO() = delete;
	static void generate(GLuint& id);
	static void setStorage(const GLuint& id, GLenum internalformat, GLsizei width, GLsizei height);
	static void bind(const GLuint& id);
	static void unbind();
	static void deleteIt(const GLuint& id);
};
