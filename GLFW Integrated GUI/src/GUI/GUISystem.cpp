#include "GUISystem.h"


GUISystem::GUISystem():state(ACTIVE), window(nullptr),
	quadEBO(0), screenQuadVBO(0), screenQuadVAO(0),canvasVBO(0),canvasVAO(0),
	mainFramebuffer(0),mainRenderbuffer(0),
	mainFramebufferTexture(nullptr),mainFramebuffeShader(nullptr),canvasShader(nullptr),colorShader(nullptr)
{}

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
	guiElements.Insert(element,[](const GUICanvas* l,const GUICanvas* r)->bool{return l->GetLayer()<r->GetLayer();});
}

void GUISystem::EnableLayerChecks(const GUILayer layer) {
	activeCheckedLayers.insert(layer);
}
void GUISystem::DisableLayerChecks(const GUILayer layer) {
	activeCheckedLayers.erase(layer);
}
bool GUISystem::IsLayerChecksEnabled(const GUILayer layer) const {
	//return allLayersEnabled || (activeCheckedLayers.count(layer)>0 && (hiddenLayerChecks || hiddenLayers.count(layer)==0));
	return layerFlags&GUI_LAYER_FLAG_ALL_LAYERS_ENABLED || (activeCheckedLayers.count(layer)>0 && (layerFlags&GUI_LAYER_FLAG_HIDDEN_LAYER_CHECKS || hiddenLayers.count(layer)==0));
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

void GUISystem::EnableLayerFlags(const unsigned flags) {
	layerFlags|=flags;
}
void GUISystem::DisableLayerFlags(const unsigned flags) {
	layerFlags&=~flags;
}
void GUISystem::ToggleLayerFlags(const unsigned flags) {
	//const unsigned int tmp = (layerFlags^flags)&flags; //to activate
	//layerFlags&=~(layerFlags&flags); //to deactivate
	//layerFlags|=tmp;

	layerFlags = (layerFlags&~(layerFlags&flags))|((layerFlags^flags)&flags);
}
unsigned GUISystem::GetLayerFlags() const {
	return layerFlags;
}


//void GUISystem::SetClickDetectionUnderLayer(const bool enabled) {
//	clickDetectUnderLayer = enabled;
//}
//void GUISystem::SetHiddenLayerChecks(const bool enabled) {
//	hiddenLayerChecks = enabled;
//}

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
	VAO::addAttrib(canvasVAO,1,2,GL_FLOAT,GL_FALSE,4*sizeof(float),(void*)(2*sizeof(float)));
	VAO::unbind();

	projection = glm::ortho(0.0f,(float)screenSize.x,(float)screenSize.y,0.0f,DEPTH_MIN,DEPTH_MAX);
	model = glm::mat4(1.0f);

	mainFramebuffeShader = new Shader("resources/shaders/GUI/mainFBOShader_vs.glsl","resources/shaders/GUI/mainFBOShader_fs.glsl");
	canvasShader = new Shader("resources/shaders/GUI/canvasShader_vs.glsl","resources/shaders/GUI/canvasShader_fs.glsl");
	colorShader = new Shader("resources/shaders/GUI/colorShader_vs.glsl","resources/shaders/GUI/colorShader_fs.glsl");
	mainFramebuffeShader->use();
	mainFramebuffeShader->setInt("screen_texture",0);
	canvasShader->use();
	canvasShader->setMat4("projection",projection);
	canvasShader->setMat4("model",model);
	canvasShader->setInt("texture_diffuse1",0);

	colorShader->use();
	colorShader->setMat4("projection",projection);

	activeCheckedLayers.insert(0);

	MarkDirty();
}

void GUISystem::checkActivateAllEventListeners() const {
	if(layerFlags&GUI_LAYER_FLAG_CLICK_DETECT_UNDER_LAYER) {
		guiElements.Traverse([](const GUICanvas* element) {
			if(!element->guiSystem->IsLayerChecksEnabled(element->GetLayer())) {
				return;
			}
			for(const auto listener: element->listeners) {
				if(listener->Check()) {
					listener->OnActivate();
				} else {
					listener->OnNotActive();
				}
			}
		});
	}else {
		GUILayer topLayer = LAYER_MIN;
		for(auto rit = guiElements.rbegin();rit!=guiElements.rend();++rit){
			const GUICanvas* element = *rit;
			if(element->GetLayer()>=topLayer) {
				for(const auto listener: element->listeners) {
					if(listener->Check()) {
						listener->OnActivate();
						topLayer = element->GetLayer();
					} else {
						listener->OnNotActive();
					}
				}
			}
		}
	}
}

void GUISystem::updateViewAllGuiElements() const {
	//int a = 0;
	for(const auto element: guiElements) {
		//std::cout<<a++<<" ";
		element->UpdateView();
	}
	//std::cout<<"\n";
}

void GUISystem::updateOnScreenSizeChange() {
	FBO::bind(mainFramebuffer);
	projection = glm::ortho(0.0f,(float)screenSize.x,(float)screenSize.y,0.0f,DEPTH_MIN,DEPTH_MAX);
	canvasShader->use();
	canvasShader->setMat4("projection",projection);
	colorShader->use();
	colorShader->setMat4("projection",projection);

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
	glDisable(GL_SCISSOR_TEST);
	glClearColor(1.0f,1.0f,1.0f,0.0f); //clear to transparent
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Draw Calls
	VAO::bind(canvasVAO);
	//EBO::bind(quadEBO);
	canvasShader->use();
	for(const auto& element: guiElements) { //?sort by layer, draw without depth test
		if(hiddenLayers.count(element->GetLayer())>0 || element->texture == nullptr){ continue; }
		canvasShader->setMat4("model",element->GetDrawModelMatrix());
		canvasShader->setVec4("colorTint",element->viewData.colorTint);
		element->texture->activate(0);
		//glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
		glDrawArrays(GL_TRIANGLES,0,6);

#ifdef DRAW_BOUNDING_BOXES
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		//glDisable(GL_DEPTH_TEST);
		const BoundingBox bb = element->GetBoundingBox();
		glm::mat4 mat = glm::mat4(1.0f);
		mat = glm::translate(mat,glm::vec3(bb.GetPosition(),DEPTH_MAX));
		mat = glm::scale(mat,glm::vec3(bb.GetSize(),0.0f));
		colorShader->use();
		colorShader->setMat4("model",mat);
		colorShader->setVec4("color",glm::vec4(1.0f));
		glDrawArrays(GL_TRIANGLES,0,6);
		//glEnable(GL_DEPTH_TEST);
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		canvasShader->use();
#endif
	}
	FBO::unbind();
	//std::cout<<"Rerenders To Framebuffer: "<<++rerendersCount<<std::endl;
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
		updateViewAllGuiElements();
		rerenderToFramebuffer();
	}

	renderFromFramebuffer();
	Undirty();
}
