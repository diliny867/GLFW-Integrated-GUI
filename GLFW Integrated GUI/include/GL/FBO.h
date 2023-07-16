#pragma once

#include <glad/glad.h>

class FBO {
public:
	FBO() = delete;
	static void generate(GLuint& id);
	static void attachRBO(GLuint rbo, GLenum attachment);
	static void attachRBO(GLuint rbo, GLenum target, GLenum attachment);
	static void attachTexture(const GLuint& textureId, GLenum attachment);
	static void attachTexture(const GLuint& textureId, GLenum target, GLenum attachment);
	static bool isComplete();
	static bool isComplete(GLenum target);
	static void bind(const GLuint& id);
	static void bind(const GLuint& id, GLenum target);
	static void unbind();
	static void unbind(GLenum target);
	static void deleteIt(const GLuint& id);
};