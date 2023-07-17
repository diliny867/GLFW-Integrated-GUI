#pragma once

//Inspired by DearImGUI


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "../../include/GL/Texture2D.h"
#include "../../include/GL/Shader.h"
#include "../../include/GL/FBO.h"
#include "../../include/GL/RBO.h"
#include "../../include/GL/VBO.h"
#include "../../include/GL/VAO.h"
#include "../../include/GL/EBO.h"

#include <vector>
#include <map>

#include "GUICommon.h"
#include "GUICanvas.h"
#include "EventListener.h"
#include "BoundingBox.h"
#include "../InputManager.h"


class GUISystem {
public:
	enum GlobalState {
		DISABLED,
		ACTIVE
	};
protected:
	GlobalState state;
	GLFWwindow* window;

	glm::ivec2 screenSize ={0,0};

	GLuint quadEBO;
	GLuint screenQuadVBO;
	GLuint screenQuadVAO;
	GLuint mainFramebuffer;
	GLuint mainRenderbuffer;
	Texture2D* mainFramebufferTexture;
	Shader* mainFramebuffeShader;

	GLuint canvasVBO;
	GLuint canvasVAO;
	Shader* canvasShader;

	//struct CanvasVertex {
	//	glm::vec2 position;
	//	glm::vec2 texCoord;
	//	CanvasVertex():position(0),texCoord(0){}
	//	CanvasVertex(const glm::vec2 position_, const glm::vec2 texCoord_):position(position_),texCoord(texCoord_){}
	//	CanvasVertex(const float x1, const float y1, const float x2, const float y2):position(x1,y1),texCoord(x2,y2){}
	//	//glm::mat4 model;
	//};

	glm::mat4 projection = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);

	bool dirty = false;

	std::vector<GUICanvas*> guiElements;


public:

	GUISystem();

	void Disable();
	void Activate();

	void MarkDirty();
	void Undirty();

	glm::ivec2 GetScreenSize() const;
	GLFWwindow* GetWindow() const;

	void Init(GLFWwindow* window_);

	void CheckActivateAllEventListeners() const;

	void UpdateOnScreenSizeChange();

	void RemapCanvasVertexBuffer();

	mutable std::size_t rerendersCount = 0; //mutable to be able to change in RerenderToFramebuffer
	void RerenderToFramebuffer() const;

	void RenderFromFramebuffer() const;

	void NewFrame();
};
