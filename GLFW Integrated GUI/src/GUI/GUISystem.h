#pragma once

//Inspired by DearImGUI

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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


class GUISystem {
protected:
	glm::mat4 projection = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);
	bool dirty = false;
public:
	GLFWwindow* window;
	std::vector<GUICanvas*> guiElements;

	GLuint quadEBO;
	GLuint screenQuadVBO;
	GLuint screenQuadVAO;
	GLuint mainFramebuffer;
	GLuint mainRenderbuffer;
	Texture2D* mainFramebufferTexture;
	Shader* mainFramebuffeShader;

	GLuint canvasVBO;
	GLuint canvasVAO;
	//struct CanvasVertex {
	//	glm::vec2 position;
	//	glm::vec2 texCoord;
	//	CanvasVertex():position(0),texCoord(0){}
	//	CanvasVertex(const glm::vec2 position_, const glm::vec2 texCoord_):position(position_),texCoord(texCoord_){}
	//	CanvasVertex(const float x1, const float y1, const float x2, const float y2):position(x1,y1),texCoord(x2,y2){}
	//	//glm::mat4 model;
	//};
	Shader* canvasShader;

	glm::ivec2 screenSize = {0,0};

	//bool keyMap[GLFW_KEY_LAST];
	//void FillKeys() {
	//	for(int key=0;key<GLFW_KEY_LAST;key++) {
	//		keyMap[key] = glfwGetKey(window,key) == GLFW_PRESS;
	//	}
	//}

	void MarkDirty() {
		dirty = true;
	}

	void Init(GLFWwindow* window_) {
		window = window_;

		glfwGetWindowSize(window,&screenSize.x,&screenSize.y);

		FBO::generate(mainFramebuffer);
		FBO::bind(mainFramebuffer);
		mainFramebufferTexture = new Texture2D(screenSize.x,screenSize.y,GL_RGBA,GL_RGBA,GL_UNSIGNED_BYTE);
		Texture2D::unbind();
		FBO::attachTexture(mainFramebufferTexture->id,GL_COLOR_ATTACHMENT0);
		//RBO::generate(mainRenderbuffer);
		//RBO::bind(mainRenderbuffer);
		//RBO::setStorage(mainRenderbuffer,GL_DEPTH24_STENCIL8,screenSize.x,screenSize.y);
		FBO::attachRBO(mainRenderbuffer,GL_DEPTH_STENCIL_ATTACHMENT);
		if(!FBO::isComplete()) {
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		}
		FBO::unbind();

		//EBO::generate(quadEBO,sizeof(quadIndices),&quadIndices,GL_STATIC_DRAW);
		//EBO::unbind();

		VBO::generate(screenQuadVBO,sizeof(quadVerticesTextureNDC),&quadVerticesTextureNDC,GL_STATIC_DRAW);
		VBO::bind(screenQuadVBO);
		VAO::generate(screenQuadVAO);
		VAO::addAttrib(screenQuadVAO,0,2,GL_FLOAT,GL_FALSE,4*sizeof(float),(void*)0);
		VAO::addAttrib(screenQuadVAO,1,2,GL_FLOAT,GL_FALSE,4*sizeof(float),(void*)(2*sizeof(float)));
		VAO::unbind();

		VBO::generate(canvasVBO,sizeof(quadVerticesTexture),&quadVerticesTexture,GL_STATIC_DRAW);
		VBO::bind(canvasVBO);
		VAO::generate(canvasVAO);
		VAO::addAttrib(canvasVAO,0,2,GL_FLOAT,GL_FALSE,4*sizeof(float),(void*)0);
		VAO::addAttrib(canvasVAO,1,2,GL_FLOAT,GL_FALSE,4*sizeof(float),(void*)(void*)(2*sizeof(float)));
		//VAO::addAttrib(canvasVAO,0,2,GL_FLOAT,GL_FALSE,sizeof(CanvasVertex),(void*)offsetof(CanvasVertex,position));
		//VAO::addAttrib(canvasVAO,1,2,GL_FLOAT,GL_FALSE,sizeof(CanvasVertex),(void*)offsetof(CanvasVertex,texCoord));
		//VAO::addAttrib(canvasVAO,2,4,GL_FLOAT,GL_FALSE,sizeof(CanvasVertex),(void*)offsetof(CanvasVertex,model));
		//VAO::setAttribDivisor(canvasVAO,1,1);
		VAO::unbind();

		projection = glm::ortho(0.0f,(float)screenSize.x,(float)screenSize.y,0.0f,-1000.0f,1000.0f);
		//projection[1][1] *= -1.0f;
		//projection[1][3] *= -1.0f;
		//projection = glm::ortho(0.0f,(float)screenSize.x,0.0f,(float)screenSize.y,-100.0f,100.0f);
		model = glm::mat4(1.0f);

		mainFramebuffeShader = new Shader("resources/shaders/GUI/mainFBOShader_vs.glsl","resources/shaders/GUI/mainFBOShader_fs.glsl");
		canvasShader = new Shader("resources/shaders/GUI/canvasShader_vs.glsl","resources/shaders/GUI/canvasShader_fs.glsl");
		mainFramebuffeShader->use();
		mainFramebuffeShader->setInt("screen_texture",0);
		canvasShader->use();
		canvasShader->setMat4("projection",projection);
		canvasShader->setMat4("model",model);
		canvasShader->setInt("texture_diffuse1",0);

		Texture2D* saulTexture = new Texture2D("resources/saul_3d.jpg",TextureType::Diffuse);

		GUICanvas* gb = new GUICanvas(0,0,400,400,this);
		gb->AddListener(new EventListener(gb,
			[](EventListener* el)->bool {
				GLFWwindow* window = el->guiElement->guiSystem->window;

				static bool clickHolding = false; //to not trigger every tick on hold

				double mouseX;
				double mouseY;
				if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
					glfwGetCursorPos(window,&mouseX,&mouseY);

					if(clickHolding == true) {
						return false;
					}
					clickHolding = true;

					if(el->guiElement->CheckClick(mouseX,mouseY)){
						return true;
					}
				} else {
					clickHolding = false;
				}
				return false;
			},
			[](EventListener* el)->void {
				el->guiElement->SetSnap(GUICanvas::RIGHT,GUICanvas::SideSnap::NO_SNAP);
				el->guiElement->boundingBox.width = 200;
				el->guiElement->SetSnap(GUICanvas::TOP,GUICanvas::SideSnap::NO_SNAP);
				el->guiElement->boundingBox.height = 400;
				el->guiElement->UpdateView();
			},
			[]() {
				std::cout<<"CLICK !!!!1 !111 ! 1 1 1 1 1  1!!!\n";
			}
			));
		gb->texture = saulTexture;
		//gb->SetSnap(GUICanvas::TOP,GUICanvas::SideSnap::WINDOW);
		//gb->SetSnap(GUICanvas::RIGHT,GUICanvas::SideSnap::WINDOW);
		gb->UpdateView();
		guiElements.push_back(gb);

		GUICanvas* gb2 = new GUICanvas(300,100,300,60,this);
		gb2->AddListener(new EventListener(gb2,
			[](EventListener* el)->bool {
				GLFWwindow* window = el->guiElement->guiSystem->window;

				static bool clickHolding = false; //to not trigger every tick on hold

				double mouseX;
				double mouseY;
				if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
					glfwGetCursorPos(window,&mouseX,&mouseY);

					if(clickHolding == true) {
						return false;
					}
					clickHolding = true;

					if(el->guiElement->CheckClick(mouseX,mouseY)){
						return true;
					}
				} else {
					clickHolding = false;
				}
				return false;
			},
			[=](EventListener* el)->void {
				gb2->SetSnap(GUICanvas::LEFT,GUICanvas::SideSnap::CANVAS,gb);
				gb2->SetSnap(GUICanvas::BOTTOM,GUICanvas::SideSnap::WINDOW);
				gb2->UpdateView();
			},
			[]() {
				std::cout<<"CLICK !!!!1 !111 ! 1 1 1 1 1  1!!!\n";
			}
			));
		gb2->texture = saulTexture;
		guiElements.push_back(gb2);

		MarkDirty();
	}

	void CheckActivateAllEventListeners() const {
		for(auto& element: guiElements) {
			for(auto& listener: element->listeners) {
				if(listener->Check()) {
					listener->OnActivate();
				}
			}
		}
	}

	void UpdateOnScreenSizeChange() {
		FBO::bind(mainFramebuffer);
		projection = glm::ortho(0.0f,(float)screenSize.x,(float)screenSize.y,0.0f,-1000.0f,1000.0f);
		//projection = glm::ortho(0.0f,(float)screenSize.x,0.0f,(float)screenSize.y,-100.0f,100.0f);
		canvasShader->use();
		canvasShader->setMat4("projection",projection);

		mainFramebufferTexture->setTexImage(screenSize.x,screenSize.y,GL_RGBA,GL_RGBA,GL_UNSIGNED_BYTE);
		//RBO::bind(mainRenderbuffer);
		//RBO::setStorage(mainRenderbuffer,GL_DEPTH24_STENCIL8,screenSize.x,screenSize.y);
		glViewport(0,0,screenSize.x,screenSize.y);
		FBO::unbind();
	}

	void RemapCanvasVertexBuffer() {
		//VBO::bind(canvasVBO);
		//std::vector<CanvasVertex> canvasVertices(guiElements.size());
		//for(const auto& element: guiElements) {
		//	BoundingBox bb = element.boundingBox;
		//	canvasVertices.push_back(CanvasVertex(bb.x,		 bb.y,		0.0f,0.0f));
		//	canvasVertices.push_back(CanvasVertex(bb.x,		 bb.GetY2(),	0.0f,1.0f));
		//	canvasVertices.push_back(CanvasVertex(bb.GetX2(),bb.GetY2(),	1.0f,1.0f));
		//	canvasVertices.push_back(CanvasVertex(bb.GetX2(),bb.y,		1.0f,0.0f));
		//}
		//VBO::setData(canvasVBO,canvasVertices.size()*sizeof(CanvasVertex),canvasVertices.data(),GL_STATIC_DRAW);
		//VBO::unbind();
	}

	mutable std::size_t rerendersCount = 0; //to be able to change in RerenderToFramebuffer
	void RerenderToFramebuffer() const {
		//set to correct framebuffer
		FBO::bind(mainFramebuffer);
		//glEnable(GL_DEPTH_TEST);
		glClearColor(1.0f,1.0f,1.0f,0.0f); //clear to transparent
		glClear(GL_COLOR_BUFFER_BIT);

		//Draw Calls
		VAO::bind(canvasVAO);
		//EBO::bind(quadEBO);
		canvasShader->use();
		for(const auto& element: guiElements) {
			canvasShader->setMat4("model",element->model);
			element->texture->activate(0);
			//glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
			glDrawArrays(GL_TRIANGLES,0,6);
		}
		FBO::unbind();
		std::cout<<"Rerenders To Framebuffer Count: "<<rerendersCount<<std::endl;
		rerendersCount++;
	}

	void RenderFromFramebuffer() const {
		//Draw screen quad
		FBO::unbind();
		glDisable(GL_DEPTH_TEST);
		VAO::bind(screenQuadVAO);
		//EBO::bind(quadEBO);
		mainFramebuffeShader->use();
		mainFramebufferTexture->bind();
		//glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
		glDrawArrays(GL_TRIANGLES,0,6);
		
	}

	void NewFrame() {
		//FillKeys();

		CheckActivateAllEventListeners();

		int screenX = 0;
		int screenY = 0;
		glfwGetWindowSize(window,&screenX,&screenY);
		if(screenX!=screenSize.x||screenY!=screenSize.y) {
			screenSize.x = screenX;
			screenSize.y = screenY;
			UpdateOnScreenSizeChange();
			for(const auto& element:guiElements) {
				element->MarkDirty(); //better mark them all dirty before, to prevent a lot of recursion (i think)
			}
			for(const auto& element:guiElements) {
				element->UpdateView();
			}
			MarkDirty();
		}

		if(dirty) {
			RerenderToFramebuffer();
		}

		RenderFromFramebuffer();
		dirty = false;
	}
};
