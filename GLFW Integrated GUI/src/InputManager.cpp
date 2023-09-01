#include "InputManager.h"


void InputManager::GLFW_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if(action == GLFW_PRESS) {
		keyMap.insert(key);
	}
	if(action == GLFW_RELEASE) {
		keyMap.erase(key);
	}
	for(const auto& callback: keyCallbacks){
		callback();
	}
}
void InputManager::GLFW_MouseCursorCallback(GLFWwindow* window, double xpos, double ypos) {
	if(mouse.firstInput) {
		mouse.posX = xpos;
		mouse.posY = ypos;
		mouse.firstInput = false;
	}
	mouse.deltaX = xpos-mouse.posX;
	mouse.deltaY = ypos-mouse.posY;
	mouse.posX = xpos;
	mouse.posY = ypos;
	for(const auto& callback: mouseCursorCallbacks){
		callback();
	}
}
void InputManager::GLFW_MouseScrollCallback(GLFWwindow* window,double xoffset,double yoffset) {
	mouse.scrollOffsetX = xoffset;
	mouse.scrollOffsetY = yoffset;
	for(const auto& callback: mouseScrollCallbacks){
		callback();
	}
}
void InputManager::GLFW_MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	if(action == GLFW_PRESS) {
		if(button == GLFW_MOUSE_BUTTON_LEFT) {
			mouse.leftPress = true;
		}
		if(button == GLFW_MOUSE_BUTTON_RIGHT) {
			mouse.rightPress = true;
		}
	}
	if(action == GLFW_RELEASE) {
		if(button == GLFW_MOUSE_BUTTON_LEFT) {
			mouse.leftPress = false;
		}
		if(button == GLFW_MOUSE_BUTTON_RIGHT) {
			mouse.rightPress = false;
		}
	}
	for(const auto& callback: mouseButtonCallbacks) {
		callback();
	}
}

void InputManager::Init(GLFWwindow* window_) {
	window = window_;
	glfwSetKeyCallback(window,GLFW_KeyCallback);
	glfwSetCursorPosCallback(window,GLFW_MouseCursorCallback);
	glfwSetScrollCallback(window,GLFW_MouseScrollCallback);
	glfwSetMouseButtonCallback(window,GLFW_MouseButtonCallback);
}

void InputManager::AddKeyCallback(const std::function<void()>& callback) {
	keyCallbacks.push_back(callback);
}
void InputManager::AddMouseCursorCallback(const std::function<void()>& callback) {
	mouseCursorCallbacks.push_back(callback);
}
void InputManager::AddMouseScrollCallback(const std::function<void()>& callback) {
	mouseScrollCallbacks.push_back(callback);
}
void InputManager::AddMouseButtonCallback(const std::function<void()>& callback) {
	mouseButtonCallbacks.push_back(callback);
}

void InputManager::PollEvents() {
	mouse.deltaX = 0; //to fix delta and scroll offset == 1 or -1 when new frame
	mouse.deltaY = 0;
	mouse.scrollOffsetX = 0;
	mouse.scrollOffsetY = 0;
	glfwPollEvents();

}
