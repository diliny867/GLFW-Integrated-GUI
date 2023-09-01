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
#include "../../include/Time.h"

#include <vector>
#include <unordered_set>
#include <algorithm>
#include <cmath>

#include "GUICommon.h"
#include "GUIObject.h"
#include "EventListener.h"
#include "GUIEventSystem.h"
#include "BoundingBox.h"
#include "../BinaryTree.h"
#include "../uniqueue.h"
//#include "../InputManager.h"

#include "GUIObjects/GUIButton.h"

//#define BB_CHECK_TRANSFORM_FOR_CLICK
#define DRAW_BOUNDING_BOXES

constexpr unsigned int GUI_LAYER_FLAG_ALL_LAYERS_ENABLED = 1<<0;
constexpr unsigned int GUI_LAYER_FLAG_CLICK_DETECT_UNDER_LAYER = 1<<1;
constexpr unsigned int GUI_LAYER_FLAG_HIDDEN_LAYER_CHECKS = 1<<2;

class GUISystem {
public:
	enum GlobalState {
		DISABLED,
		ACTIVE
	};
private:
	friend GUIObject;

	GlobalState state;
	GLFWwindow* window;

	glm::ivec2 screenSize = {0,0};

	GLuint quadEBO;
	GLuint screenQuadVBO;
	GLuint screenQuadVAO;
	GLuint canvasVBO;
	GLuint canvasVAO;

	GLuint mainFramebuffer;
	GLuint mainRenderbuffer;
	Texture2D* mainFramebufferTexture;
	Shader* mainFramebuffeShader;
	Shader* canvasShader;
	Shader* colorShader;

	glm::mat4 projection = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);

	bool dirty = false;

	std::vector<GUIObject*> guiElements;

	//BinaryTree<GUICanvas*> guiElements;

	uniqueue<GUIObject*> updateQueue;

	std::unordered_set<GUILayer> activeCheckedLayers;
	std::unordered_set<GUILayer> hiddenLayers;

	unsigned int layerFlags = GUI_LAYER_FLAG_ALL_LAYERS_ENABLED | GUI_LAYER_FLAG_CLICK_DETECT_UNDER_LAYER;

	void checkActivateAllEventListeners() const;
	void updateViewAllGuiElements();

	void updateOnScreenSizeChange();
	//std::size_t rerendersCount = 0; //mutable to be able to change in RerenderToFramebuffer
	void rerenderToFramebuffer() const;
	void renderFromFramebuffer() const;
public:

	GUISystem();

	void Deactivate();
	void Activate();

	void MarkDirty();
	void Undirty();

	glm::ivec2 GetScreenSize() const;
	GLFWwindow* GetWindow() const;

	void AddCanvasElement(GUIObject* element);
	void RemoveCanvasElement(const GUIObject* element);

	void EnableLayerChecks(const GUILayer layer);
	void DisableLayerChecks(const GUILayer layer);
	bool IsLayerChecksEnabled(const GUILayer layer) const;
	void HideLayer(const GUILayer layer); //Do note that layer still checks its listeners
	void ShowLayer(const GUILayer layer);
	bool IsLayerHidden(const GUILayer layer) const;

	void EnableLayerFlags(unsigned int flags);
	void DisableLayerFlags(unsigned int flags);
	void ToggleLayerFlags(unsigned int flags);
	unsigned int GetLayerFlags() const;

	void Init(GLFWwindow* window_);

	void NewFrame();
};