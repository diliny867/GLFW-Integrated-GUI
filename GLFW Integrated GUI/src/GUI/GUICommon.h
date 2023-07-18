#pragma once

typedef int GUILayer;

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