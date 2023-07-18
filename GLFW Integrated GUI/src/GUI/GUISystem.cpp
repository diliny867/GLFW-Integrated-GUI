#include "GUISystem.h"


GUISystem::GUISystem():state(ACTIVE) {}

void GUISystem::Deactivate() {
	state = DISABLED;
}
void GUISystem::Activate() {
	state = ACTIVE;
}

void GUISystem::MarkDirty(){
	dirty = true;
}
void GUISystem::Undirty() {
	dirty = false;
}

glm::ivec2 GUISystem::GetScreenSize() const {
	return screenSize;
}
GLFWwindow* GUISystem::GetWindow() const {
	return window;
}

void GUISystem::AddCanvasElement(GUICanvas* element) {
	element->guiSystem = this;
	element->MarkDirty();
	guiElements.push_back(element);
}

void GUISystem::EnableLayerChecks(const GUILayer layer) {
	activeCheckedLayers.insert(layer);
}
void GUISystem::DisableLayerChecks(const GUILayer layer) {
	activeCheckedLayers.erase(layer);
}
bool GUISystem::IsLayerChecksEnabled(const GUILayer layer) const {
	return activeCheckedLayers.count(layer)>0 && (hiddenLayerChecks || hiddenLayers.count(layer)==0);
}

void GUISystem::HideLayer(const GUILayer layer) {
	hiddenLayers.insert(layer);
}
void GUISystem::ShowLayer(const GUILayer layer) {
	hiddenLayers.erase(layer);
}
bool GUISystem::IsLayerHidden(const GUILayer layer) const {
	return hiddenLayers.count(layer)>0;
}



void GUISystem::SetClickDetectionUnderLayer(const bool enabled) {
	clickDetectUnderLayer = enabled;
}
void GUISystem::SetHiddenLayerChecks(const bool enabled) {
	hiddenLayerChecks = enabled;
}

void GUISystem::Init(GLFWwindow* window_) {
	window = window_;

	glfwGetWindowSize(window,&screenSize.x,&screenSize.y);

	FBO::generate(mainFramebuffer);
	FBO::bind(mainFramebuffer);
	mainFramebufferTexture = new Texture2D(screenSize.x,screenSize.y,GL_RGBA,GL_RGBA,GL_UNSIGNED_BYTE);
	Texture2D::unbind();
	FBO::attachTexture(mainFramebufferTexture->id,GL_COLOR_ATTACHMENT0);
	RBO::generate(mainRenderbuffer);
	RBO::bind(mainRenderbuffer);
	RBO::setStorage(mainRenderbuffer,GL_DEPTH24_STENCIL8,screenSize.x,screenSize.y);
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

	activeCheckedLayers.insert(0);

	MarkDirty();
}

void GUISystem::checkActivateAllEventListeners() const {
	if(clickDetectUnderLayer) {
		for(const auto& element: guiElements) {
			if(!IsLayerChecksEnabled(element->layer)) {
				continue;
			}
			for(const auto& listener: element->listeners) {
				if(listener->Check()) {
					listener->OnActivate();
				}
			}
		}
	}else {
		EventListener* topListener = nullptr;
		GUILayer topLayer = INT_MIN;
		for(const auto& element: guiElements) {
			if(!IsLayerChecksEnabled(element->layer)) {
				continue;
			}
			for(const auto& listener: element->listeners) {
				if(listener->Check()) {
					if(element->layer>topLayer) {
						topListener = listener;
						topLayer = element->layer;
					}
				}
			}
		}
		if(topListener!=nullptr) {
			topListener->OnActivate();
		}
	}
}

void GUISystem::updateViewAllGuiElements() const {
	for(const auto& element: guiElements) {
		element->UpdateView();
	}
}

void GUISystem::updateOnScreenSizeChange() {
	FBO::bind(mainFramebuffer);
	projection = glm::ortho(0.0f,(float)screenSize.x,(float)screenSize.y,0.0f,-1000.0f,1000.0f);
	//projection = glm::ortho(0.0f,(float)screenSize.x,0.0f,(float)screenSize.y,-100.0f,100.0f);
	canvasShader->use();
	canvasShader->setMat4("projection",projection);

	mainFramebufferTexture->setTexImage(screenSize.x,screenSize.y,GL_RGBA,GL_RGBA,GL_UNSIGNED_BYTE);
	RBO::bind(mainRenderbuffer);
	RBO::setStorage(mainRenderbuffer,GL_DEPTH24_STENCIL8,screenSize.x,screenSize.y);
	glViewport(0,0,screenSize.x,screenSize.y);
	FBO::unbind();
}

void GUISystem::rerenderToFramebuffer() const {
	//Set to correct framebuffer
	FBO::bind(mainFramebuffer);
	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0f,1.0f,1.0f,0.0f); //clear to transparent
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Draw Calls
	VAO::bind(canvasVAO);
	//EBO::bind(quadEBO);
	canvasShader->use();
	for(const auto& element: guiElements) {
		if(hiddenLayers.count(element->layer)>0 || element->texture == nullptr){ continue; }
		canvasShader->setMat4("model",element->model);
		canvasShader->setFloat("layer",(float)element->layer);
		element->texture->activate(0);
		//glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
		glDrawArrays(GL_TRIANGLES,0,6);
	}
	FBO::unbind();
	std::cout<<"Rerenders To Framebuffer: "<<++rerendersCount<<std::endl;
}

void GUISystem::renderFromFramebuffer() const {
	//Draw framebuffer to screen quad
	FBO::unbind();
	glDisable(GL_DEPTH_TEST);
	VAO::bind(screenQuadVAO);
	//EBO::bind(quadEBO);
	mainFramebuffeShader->use();
	mainFramebufferTexture->bind();
	//glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
	glDrawArrays(GL_TRIANGLES,0,6);
		
}

void GUISystem::NewFrame() {
	if(state == DISABLED) {
		return;
	}

	checkActivateAllEventListeners();
	updateViewAllGuiElements();

	int screenX = 0;
	int screenY = 0;
	glfwGetWindowSize(window,&screenX,&screenY);
	if(screenX!=screenSize.x||screenY!=screenSize.y) {
		screenSize.x = screenX;
		screenSize.y = screenY;
		updateOnScreenSizeChange();
		for(const auto& element:guiElements) {
			element->MarkDirty(); //better mark them all dirty before, to prevent a lot of recursion (i think)
		}
		for(const auto& element:guiElements) {
			element->UpdateView();
		}
		MarkDirty();
	}

	if(dirty) {
		rerenderToFramebuffer();
	}

	renderFromFramebuffer();
	Undirty();
}
