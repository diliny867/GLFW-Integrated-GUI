#include <glad/glad.h>//should always be first
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>//OpenGL Mathematics
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../include/glm_useful_includes.hpp"

#include <iostream>
#include <functional>

#include "../include/GL/VBO.h"
#include "../include/GL/VAO.h"
#include "../include/GL/Camera.h"
#include "../include/GL/Shader.h"
#include "../include/Time.h"

#include "InputManager.h"
#include "GUI/GUISystem.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback();
void mouse_cursor_callback();
void mouse_scroll_callback();
void mouse_button_callback();
std::function<void()> shader_viewpoint_callback;
void printFPS();

float SCR_WIDTH = 800;
float SCR_HEIGHT = 600;

Camera camera(glm::vec3(0.0f,0.0f,0.0f));

GUISystem mainGUI;

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//opengl versions
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//set to core profile

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH,SCR_HEIGHT, "OpenGL application", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSwapInterval(0);//VSync
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//capture mouse

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
	//glEnable(GL_LINE_SMOOTH);
    //glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);

    Time::Init();
    InputManager::Init(window);
    InputManager::SetKeyCallback(key_callback);
    InputManager::SetMouseButtonCallback(mouse_button_callback);
    InputManager::SetMouseCursorCallback(mouse_cursor_callback);
    InputManager::SetMouseScrollCallback(mouse_scroll_callback);
    InputManager::mouse.posX = SCR_WIDTH/2;
    InputManager::mouse.posY = SCR_HEIGHT/2;

    glm::mat4 projection = glm::ortho(0.0f,SCR_WIDTH,SCR_HEIGHT,0.0f,DEPTH_MIN,DEPTH_MAX);
    glm::mat4 model = glm::mat4(1.0f);

    shader_viewpoint_callback = [&]() {
        projection = glm::ortho(0.0f,SCR_WIDTH,SCR_HEIGHT,0.0f,DEPTH_MIN,DEPTH_MAX);
    };

    Texture2D* saulTexture = new Texture2D("resources/saul_3d.jpg",TextureType::Diffuse);
    Texture2D* sillyTexture = new Texture2D("resources/silly.jpg",TextureType::Diffuse);
    Texture2D* earthBurnTexture = new Texture2D("resources/earth_burn.png",TextureType::Diffuse);
    Texture2D* whiteTexture = new Texture2D("resources/white.png",TextureType::Diffuse);
    Texture2D* vesselsBgTexture = new Texture2D("resources/vessels_bg.png",TextureType::Diffuse);
    
    mainGUI.Init(window);
    mainGUI.EnableLayerFlags(GUI_LAYER_FLAG_ALL_LAYERS_ENABLED);
    mainGUI.DisableLayerFlags(GUI_LAYER_FLAG_CLICK_DETECT_UNDER_LAYER);
    //mainGUI.SetClickDetectionUnderLayer(false);
    //mainGUI.allLayersEnabled = true;

    GUIButton* exitButton = new GUIButton(0,0,50,50);
    exitButton->SetOnLeftMouseDown([&window](GUIObject* obj)->void {
			std::cout<<"Exiting...\n";
			glfwSetWindowShouldClose(window,true);
    });
    float exitButtonAngle = 0.0f;
    exitButton->SetOnMouseHover([&exitButtonAngle](GUIObject* obj) {
        constexpr float sizeChange = 1.1f;
        constexpr float sizeOffset = (1.0f-sizeChange)/2.0f;
        obj->localViewTransformMat4 = glm::translate(glm::mat4(1.0f),glm::vec3(sizeOffset,sizeOffset,0.0f));
        obj->localViewTransformMat4 = glm::scale(obj->localViewTransformMat4,glm::vec3(sizeChange,sizeChange,1.0f));
        obj->localViewTransformMat4 = glm::rotateAroundCenter(obj->localViewTransformMat4,exitButtonAngle,glm::vec3(0.0f,0.0f,1.0f));
        exitButtonAngle += 0.001f;
        obj->MarkDirty();
        std::cout<<"HOVER\n";
    });
    exitButton->SetOnMouseHoverElse([&exitButtonAngle](GUIObject* obj) {
        if(obj->localViewTransformMat4 != glm::mat4(1.0f)){
            obj->localViewTransformMat4 = glm::mat4(1.0f);
            obj->localViewTransformMat4 = glm::rotateAroundCenter(obj->localViewTransformMat4,exitButtonAngle,glm::vec3(0.0f,0.0f,1.0f));
            obj->MarkDirty();
        }
    });
    exitButton->SetSnap(GUIObject::RIGHT,GUIObject::SideSnap::DRAG,GUIObject::SideSnap::WINDOW);
    exitButton->SetSnap(GUIObject::TOP,GUIObject::SideSnap::DRAG,GUIObject::SideSnap::WINDOW);
    exitButton->SetTexture(earthBurnTexture);
    exitButton->SetLayer(1);
    mainGUI.AddCanvasElement(exitButton);

    GUIButton* gb = new GUIButton(0,0,400,400);
    glm::vec2 saulMouseOffset = glm::vec2(0.0f);
    //gb->SetOnLeftMouseDown([&](GUIObject* obj)->void {
    //    //element->transform.size.x *= 0.95f;
    //    //element->transform.size.y += 5;
    //    saulMouseOffset = glm::vec2(InputManager::mouse.posX,InputManager::mouse.posY) - glm::vec2(obj->transform.position);
    //    obj->MarkDirty();
    //    std::cout<<"CLICK\n";
    //});

    gb->AddListener(new EventListener(
        EL_Check_MouseDownLMB_Func,
        [&](GUIObject* obj)->void {
            //element->transform.size.x *= 0.95f;
			//element->transform.size.y += 5;
            saulMouseOffset = glm::vec2(InputManager::mouse.posX,InputManager::mouse.posY) - glm::vec2(obj->transform.position);
            obj->MarkDirty();
            std::cout<<"CLICK\n";
        }
        ));
    gb->AddListener(new EventListener(
        EL_Check_StartHereHoldUntilReleaseLMB_Func,
        [&saulMouseOffset](GUIObject* obj)->void {
            const InputManager::Mouse& mouse = InputManager::mouse;
            const glm::vec2 mousePos = glm::vec2(mouse.posX,mouse.posY);

            obj->transform.position = glm::vec3(mousePos - saulMouseOffset,obj->transform.position.z);

            obj->MarkDirty();
        }
    ));
    //gb->SetSnap(GUICanvas::LEFT,GUICanvas::SideSnap::SCALE,GUICanvas::SideSnap::WINDOW);
    //gb->SetSnap(GUICanvas::TOP,GUICanvas::SideSnap::DRAG,GUICanvas::SideSnap::WINDOW);
    gb->SetTexture(saulTexture);
    mainGUI.AddCanvasElement(gb);

    GUIObject* gb2 = new GUIObject(300,100,300,60);
    gb2->AddListener(new EventListener(
        EL_Check_StartHereHoldUntilReleaseLMB_Func,
        [](GUIObject* obj)->void {
            const InputManager::Mouse& mouse = InputManager::mouse;
            obj->transform.position.y += mouse.deltaY;
            obj->MarkDirty();
            std::cout<<"LEFT HOLD\n";
        }
        ));
    gb2->AddListener(new EventListener(
        EL_Check_StartHereHoldUntilReleaseRMB_Func,
        [](GUIObject* obj)->void {
			const InputManager::Mouse& mouse = InputManager::mouse;
            if(mouse.leftPress){ return; }
            obj->transform.position.y += mouse.deltaY;
            obj->MarkDirty();
            std::cout<<"RIGHT HOLD\n";
        }
		));
    gb2->SetTexture(sillyTexture);
    gb2->SetSnap(GUIObject::LEFT,GUIObject::SideSnap::SCALE,GUIObject::SideSnap::CANVAS,gb);
    gb2->SetSnap(GUIObject::RIGHT,GUIObject::SideSnap::SCALE,GUIObject::SideSnap::WINDOW);
    gb2->SetLayer(-1);
    mainGUI.AddCanvasElement(gb2);

    GUIObject* gb3 = new GUIObject(600,400,100,100);
    gb3->AddListener(new EventListener(
		EL_Check_StartHereHoldUntilReleaseLMB_Func,
        [](GUIObject* obj)->void {
            const InputManager::Mouse& mouse = InputManager::mouse;
            obj->transform.position += glm::vec3(mouse.deltaX,mouse.deltaY,0.0f);
            const glm::vec2 center = obj->transform.position;
            obj->transform.rotation = glm::angleAxis(10.0f*(float)Time::deltaTime,glm::vec3(0.0f,0.0f,1.0f))*obj->transform.rotation;
            obj->MarkDirty();
            std::cout<<"LEFT HOLD\n";
        }
    ));
    gb3->AddListener(new EventListener(
        EL_Check_StartHereHoldUntilReleaseRMB_Func,
        [](GUIObject* obj)->void {
            const InputManager::Mouse& mouse = InputManager::mouse;
            if(!mouse.leftPress) {
                obj->transform.position += glm::vec3(mouse.deltaX,mouse.deltaY,0.0f);
            }
            obj->transform.rotation = glm::angleAxis(-10.0f*(float)Time::deltaTime,glm::vec3(0.0f,0.0f,1.0f))*obj->transform.rotation;
            obj->MarkDirty();
            std::cout<<"RIGHT HOLD\n";
        }
	));
    gb3->SetTexture(whiteTexture);
    gb3->transform.position.z = 1.0f;
    mainGUI.AddCanvasElement(gb3);

    GUIObject* bg = new GUIObject();
    bg->SetSnap(GUIObject::LEFT,GUIObject::SideSnap::SCALE,GUIObject::SideSnap::WINDOW);
    bg->SetSnap(GUIObject::RIGHT,GUIObject::SideSnap::SCALE,GUIObject::SideSnap::WINDOW);
    bg->SetSnap(GUIObject::TOP,GUIObject::SideSnap::SCALE,GUIObject::SideSnap::WINDOW);
    bg->SetSnap(GUIObject::BOTTOM,GUIObject::SideSnap::SCALE,GUIObject::SideSnap::WINDOW);
    bg->SetTexture(vesselsBgTexture);
    bg->viewData.colorTint = glm::vec4(glm::vec3(0.3f),1.0f);
    //mainGUI.AddCanvasElement(bg);

    //mainGUI.guiElements.Traverse([](const GUICanvas* c){std::cout<<c->GetLayer()<<" ";});

    //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

    while (!glfwWindowShouldClose(window)) {
        Time::Update();
        InputManager::PollEvents();

        //printFPS();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT/* | GL_STENCIL_BUFFER_BIT*/);

        mainGUI.NewFrame();

        glfwSwapBuffers(window);
    }
    
    glfwTerminate();
    return 0;
}

void printFPS() {
    std::cout << 1/Time::deltaTime << std::endl;
}

void key_callback() {
    if(InputManager::keyMap.contains(GLFW_KEY_ESCAPE)) {
        mainGUI.Deactivate();
    }
    if(InputManager::keyMap.contains(GLFW_KEY_ENTER)) {
        mainGUI.Activate();
    }
}

void mouse_cursor_callback() {
    
}

void mouse_button_callback() {
    
}

void mouse_scroll_callback() {

}


void framebuffer_size_callback(GLFWwindow* window,int width,int height) {
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
    glViewport(0,0,width,height); //0,0 - left bottom
    shader_viewpoint_callback();
}