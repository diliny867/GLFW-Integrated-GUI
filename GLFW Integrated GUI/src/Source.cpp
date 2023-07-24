#include <glad/glad.h>//should always be first
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>//OpenGL Mathematics
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL application", NULL, NULL);
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

    glm::mat4 projection = glm::ortho(0.0f,SCR_WIDTH,SCR_HEIGHT,0.0f,-1000.0f,1000.0f);
    glm::mat4 model = glm::mat4(1.0f);

    shader_viewpoint_callback = [&]() {
        projection = glm::ortho(0.0f,SCR_WIDTH,SCR_HEIGHT,0.0f,-1000.0f,1000.0f);
    };

    Texture2D* saulTexture = new Texture2D("resources/saul_3d.jpg",TextureType::Diffuse);
    Texture2D* sillyTexture = new Texture2D("resources/silly.jpg",TextureType::Diffuse);
    Texture2D* earthBurnTexture = new Texture2D("resources/earth_burn.png",TextureType::Diffuse);

    mainGUI.Init(window);
    mainGUI.SetClickDetectionUnderLayer(false);

    GUICanvas* exitButton = new GUICanvas(0,0,50,50,&mainGUI);
    exitButton->AddListener(new EventListener(
        EL_Check_SingleClick_Func,
        [](EventListener* el)->void {
        },
        [&]() {
            std::cout<<"Exiting...\n";
            glfwSetWindowShouldClose(window,true);
        }
    ));
    exitButton->SetSnap(GUICanvas::RIGHT,GUICanvas::SideSnap::DRAG,GUICanvas::SideSnap::WINDOW);
    exitButton->SetSnap(GUICanvas::TOP,GUICanvas::SideSnap::DRAG,GUICanvas::SideSnap::WINDOW);
    exitButton->SetTexture(earthBurnTexture);
    exitButton->layer = 1;
    exitButton->AddListener(new EventListener(
        EL_Check_OnHover_Func,
        [](const EventListener* el) {
            GUICanvas* element = el->guiElement;
			constexpr float sizeChange = 1.1f;
			constexpr float sizeOffset = (1.0f-sizeChange)/2.0f;
            element->viewTransform = glm::translate(glm::mat4(1.0f),glm::vec3(sizeOffset,sizeOffset,0.0f));
            element->viewTransform = glm::scale(element->viewTransform,glm::vec3(sizeChange,sizeChange,1.0f));
            element->MarkDirty();
        },
        [](const EventListener* el) {
            GUICanvas* element = el->guiElement;
            if(element->viewTransform != glm::mat4(1.0f)){
                element->viewTransform = glm::mat4(1.0f);
                element->MarkDirty();
            }
        },
        []() {
            std::cout<<"HOVER\n";
        }
    ));
    mainGUI.AddCanvasElement(exitButton);
    mainGUI.EnableLayerChecks(1);

    GUICanvas* gb = new GUICanvas(0,0,400,400,&mainGUI);
    gb->AddListener(new EventListener(
        EL_Check_SingleClick_Func,
        [](const EventListener* el)->void {
            el->guiElement->transform.size.x *= 0.95f;
			el->guiElement->transform.size.y += 5;
			el->guiElement->MarkDirty();
        },
        []() {
            std::cout<<"CLICK\n";
        }
        ));
    gb->SetTexture(saulTexture);
    gb->UpdateView();
    mainGUI.AddCanvasElement(gb);

    GUICanvas* gb2 = new GUICanvas(300,100,300,60,&mainGUI);
    gb2->AddListener(new EventListener(
        EL_Check_HoldUntilRelease_Func,
        [](const EventListener* el)->void {
            const InputManager::Mouse& mouse = InputManager::mouse;
        },
        []() {
            std::cout<<"LEFT HOLD\n";
        }
        ));
    gb2->AddListener(new EventListener(
        EL_Check_HoldUntilReleaseRMB_Func,
        [](const EventListener* el)->void {
			const InputManager::Mouse& mouse = InputManager::mouse;
        },
        []() {
            std::cout<<"RIGHT HOLD\n";
        }
		));
    gb2->SetTexture(sillyTexture);
    gb2->SetSnap(GUICanvas::LEFT,GUICanvas::SideSnap::SCALE,GUICanvas::SideSnap::CANVAS,gb);
    gb2->SetSnap(GUICanvas::RIGHT,GUICanvas::SideSnap::SCALE,GUICanvas::SideSnap::WINDOW);
    gb2->MarkDirty();
    gb2->UpdateView();
    mainGUI.AddCanvasElement(gb2);


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