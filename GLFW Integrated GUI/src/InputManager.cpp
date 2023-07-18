#include "InputManager.h"


void InputManager::GLFW_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if(action == GLFW_PRESS) {
		keyMap.insert(key);
	}
	if(action == GLFW_RELEASE) {
		keyMap.erase(key);
	}
	if(keyCallback!=nullptr){
		keyCallback();
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
	if(mouseCursorCallback!=nullptr){
		mouseCursorCallback();
	}
}
void InputManager::GLFW_MouseScrollCallback(GLFWwindow* window,double xoffset,double yoffset) {
	mouse.scrollOffsetX = xoffset;
	mouse.scrollOffsetY = yoffset;
	if(mouseScrollCallback!=nullptr){
		mouseScrollCallback();
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
	if(mouseButtonCallback!=nullptr){
		mouseButtonCallback();
	}
}

void InputManager::Init(GLFWwindow* window_) {
	window = window_;
	glfwSetKeyCallback(window,GLFW_KeyCallback);
	glfwSetCursorPosCallback(window,GLFW_MouseCursorCallback);
	glfwSetScrollCallback(window,GLFW_MouseScrollCallback);
	glfwSetMouseButtonCallback(window,GLFW_MouseButtonCallback);
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
	mouse.deltaX = 0; //to fix delta and scroll offset == 1 or -1 when new frame
	mouse.deltaY = 0;
	mouse.scrollOffsetX = 0;
	mouse.scrollOffsetY = 0;
	glfwPollEvents();
	if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		mouse.leftPress = true;
	}else {
		mouse.leftPress = false;
	}
	if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		mouse.rightPress = true;
	} else {
		mouse.rightPress = false;
	}

}
