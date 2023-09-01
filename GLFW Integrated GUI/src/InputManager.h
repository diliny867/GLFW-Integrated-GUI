#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <iostream>
#include <vector>
#include <unordered_set>
#include <functional>


typedef int GLFWKeyCode;
class KeyMap {
private:
    std::unordered_set<GLFWKeyCode> keyMap;
public:
	bool contains(const GLFWKeyCode key) const {
        return keyMap.count(key)>0;
	}
    void insert(const GLFWKeyCode key) {
        keyMap.emplace(key);
	}
    void erase(const GLFWKeyCode key) {
        keyMap.erase(key);
	}
};

class InputManager {
private:
    static void GLFW_KeyCallback(GLFWwindow* window,int key,int scancode,int action,int mods);
    static void GLFW_MouseCursorCallback(GLFWwindow* window,double xpos,double ypos);
    static void GLFW_MouseScrollCallback(GLFWwindow* window,double xoffset,double yoffset);
    static void GLFW_MouseButtonCallback(GLFWwindow* window,int button,int action,int mods);

    //typedef void (*keyfun)(int key,int scancode,int action,int mods);
    //typedef void (*cursorposfun)(double xpos,double ypos);
    //typedef void (*scrollfun)(double xoffset,double yoffset);
    //typedef void (*mousebuttonfun)(int button,int action,int mods);
	//static keyfun KeyCallback;
    //static cursorposfun MouseCursorCallback;
    //static scrollfun MouseScrollCallback;
    //static mousebuttonfun MouseButtonCallback;
    inline static std::vector<std::function<void()>> keyCallbacks;
    inline static std::vector<std::function<void()>> mouseCursorCallbacks;
    inline static std::vector<std::function<void()>> mouseScrollCallbacks;
    inline static std::vector<std::function<void()>> mouseButtonCallbacks;
public:
    class Mouse {
    private:
        bool firstInput = true;
    public:
        Mouse(const double posX_,const double posY_):posX(posX_),posY(posY_){}
        Mouse():Mouse(0,0){}
        double posX=0;
        double posY=0;
        double deltaX=0;
        double deltaY=0;
        double scrollOffsetX=0;
        double scrollOffsetY=0;
        bool leftPress = false;
        bool rightPress = false;
        friend InputManager;
    };
    inline static GLFWwindow* window;
    inline static KeyMap keyMap; //Stores GLFW Keycodes
    inline static Mouse mouse;
    static void Init(GLFWwindow* window_);
    //static void SetKeyCallback(keyfun callback);
    //static void SetMouseCursorCallback(cursorposfun callback);
    //static void SetMouseScrollCallback(scrollfun callback);
    //static void SetMouseButtonCallback(mousebuttonfun callback);
    static void AddKeyCallback(const std::function<void()>& callback);
    static void AddMouseCursorCallback(const std::function<void()>& callback);
    static void AddMouseScrollCallback(const std::function<void()>& callback);
    static void AddMouseButtonCallback(const std::function<void()>& callback);
    static void PollEvents();
};