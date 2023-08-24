#pragma once

#include <glm/glm.hpp>

typedef int GUILayer;

constexpr GUILayer LAYER_MAX = 1000;
constexpr GUILayer LAYER_MIN = -1000;
constexpr float DEPTH_MAX = (float)LAYER_MAX;
constexpr float DEPTH_MIN = (float)LAYER_MIN;

inline glm::vec2 minMaxFrom4(const float a,const float b,const float c,const float d) {
	const float min1 = (a < b) ? a : b;
	const float min2 = (c < d) ? c : d;

	const float max1 = (a > b) ? a : b;
	const float max2 = (c > d) ? c : d;

	return glm::vec2((min1 < min2) ? min1 : min2,(max1 > max2) ? max1 : max2);
}

constexpr unsigned int quadIndices[] = {
	0, 1, 2,
	2, 3, 0
};

constexpr float quadVerticesNDC[] = {
	-1.0f, -1.0f,
	-1.0f,  1.0f,
	 1.0f,  1.0f,
	-1.0f, -1.0f,
	 1.0f,  1.0f,
	 1.0f, -1.0f
};
constexpr float quadVertices[] = {
	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 1.0f,
	1.0f, 0.0f
};

constexpr float quadVerticesTextureNDC[] ={
	-1.0f, -1.0f,  0.0f, 0.0f,
	-1.0f,  1.0f,  0.0f, 1.0f,
	 1.0f,  1.0f,  1.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	 1.0f,  1.0f,  1.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f
};
constexpr float quadVerticesTexture[] ={
	0.0f, 0.0f,  0.0f, 0.0f,
	0.0f, 1.0f,  0.0f, 1.0f,
	1.0f, 1.0f,  1.0f, 1.0f,
	0.0f, 0.0f,  0.0f, 0.0f,
	1.0f, 1.0f,  1.0f, 1.0f,
	1.0f, 0.0f,  1.0f, 0.0f
};


constexpr float quadVerticesNDC_EBO[] ={
	-1.0f, -1.0f,
	-1.0f,  1.0f,
	 1.0f,  1.0f,
	 1.0f, -1.0f
};
constexpr float quadVertices_EBO[] ={
	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f
};

constexpr float quadVerticesTextureNDC_EBO[] ={
	-1.0f, -1.0f,  0.0f, 0.0f,
	-1.0f,  1.0f,  0.0f, 1.0f,
	 1.0f,  1.0f,  1.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f
};
constexpr float quadVerticesTexture_EBO[] ={
	0.0f, 0.0f,  0.0f, 0.0f,
	0.0f, 1.0f,  0.0f, 1.0f,
	1.0f, 1.0f,  1.0f, 1.0f,
	1.0f, 0.0f,  1.0f, 0.0f
};