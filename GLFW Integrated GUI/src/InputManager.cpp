#include "InputManager.h"


void InputManager::GLFW_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if(action == GLFW_PRESS) {
		KeyMap.emplace(key);
	}
	if(action == GLFW_RELEASE) {
		KeyMap.erase(key);
	}
	if(keyCallback!=nullptr){
		keyCallback();
	}
}
void InputManager::GLFW_MouseCursorCallback(GLFWwindow* window, double xpos, double ypos) {
	if(Mouse.firstInput) {
		Mouse.posX = xpos;
		Mouse.posY = ypos;
		Mouse.firstInput = false;
	}
	Mouse.deltaX = Mouse.posX-xpos;
	Mouse.deltaY = Mouse.posY-ypos;
	Mouse.posX = xpos;
	Mouse.posY = ypos;
	if(mouseCursorCallback!=nullptr){
		mouseCursorCallback();
	}
}
void InputManager::GLFW_MouseScrollCallback(GLFWwindow* window,double xoffset,double yoffset) {
	Mouse.scrollOffsetX = xoffset;
	Mouse.scrollOffsetY = yoffset;
	if(mouseScrollCallback!=nullptr){
		mouseScrollCallback();
	}
}
void InputManager::GLFW_MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	if(action == GLFW_PRESS) {
		if(button == GLFW_MOUSE_BUTTON_LEFT) {
			Mouse.leftPress = true;
		}
		if(button == GLFW_MOUSE_BUTTON_RIGHT) {
			Mouse.rightPress = true;
		}
	}
	if(action == GLFW_RELEASE) {
		if(button == GLFW_MOUSE_BUTTON_LEFT) {
			Mouse.leftPress = false;
		}
		if(button == GLFW_MOUSE_BUTTON_RIGHT) {
			Mouse.rightPress = false;
		}
	}
	if(mouseButtonCallback!=nullptr){
		mouseButtonCallback();
	}
}

void InputManager::Init(GLFWwindow* window_) {
	Window = window_;
	glfwSetKeyCallback(Window,GLFW_KeyCallback);
	glfwSetCursorPosCallback(Window,GLFW_MouseCursorCallback);
	glfwSetScrollCallback(Window,GLFW_MouseScrollCallback);
	glfwSetMouseButtonCallback(Window,GLFW_MouseButtonCallback);
}

void InputManager::SetKeyCallback(const std::function<void()>& callback) {
	keyCallback = callback;
}
void InputManager::SetMouseCursorCallback(const std::function<void()>& callback) {
	mouseCursorCallback = callback;
}
void InputManager::SetMouseScrollCallback(const std::function<void()>& callback) {
	mouseScrollCallback = callback;
}
void InputManager::SetMouseButtonCallback(const std::function<void()>& callback) {
	mouseButtonCallback = callback;
}

void InputManager::PollEvents() {
	Mouse.deltaX = 0; //to fix delta == 1 or -1 when new frame
	Mouse.deltaY = 0;
	glfwPollEvents();
	if(glfwGetMouseButton(Window,GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		Mouse.leftPress = true;
	}else {
		Mouse.leftPress = false;
	}
	if(glfwGetMouseButton(Window,GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		Mouse.rightPress = true;
	} else {
		Mouse.rightPress = false;
	}
	
}
