#pragma once

#include <glad/glad.h>
#include "stb_image.h"

#include <iostream>

enum TextureType {
	Diffuse,
	Specular,
	Normal,
	Height
};

class Texture2D {
public:
	GLuint id{};
	TextureType type;
	std::string path;
	Texture2D(){}
	Texture2D(int width, int height);
	Texture2D(int width, int height, GLint internalformat, GLenum format, GLenum dataType);
	Texture2D(int width, int height, GLint internalformat, GLenum format, GLenum dataType, const void* data);
	Texture2D(const char* name, TextureType type);
	void activate(GLenum textureUnit) const;
	static void deactivate();
	void setTexImage(int width, int height, GLint internalformat, GLenum format, GLenum dataType) const;
	void setParameteri(GLenum parameterName, GLint parameter) const;
	void bind() const;
	static void unbind();
};