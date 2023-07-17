#pragma once

#include <GLFW/glfw3.h>


#include <iostream>
#include <unordered_set>
#include <functional>

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
    inline static std::function<void()> keyCallback = nullptr;
    inline static std::function<void()> mouseCursorCallback = nullptr;
    inline static std::function<void()> mouseScrollCallback = nullptr;
    inline static std::function<void()> mouseButtonCallback = nullptr;
public:
    inline static std::unordered_set<int> KeyMap;

    class Mouse {
    private:
        bool firstInput = true;
    public:
        Mouse(const double posX_,const double posY_):posX(posX_),posY(posY_){}
        Mouse():Mouse(0,0){}
        double posX;
        double posY;
        double deltaX;
        double deltaY;
        double scrollOffsetX;
        double scrollOffsetY;
        bool leftPress = false;
        bool rightPress = false;
        friend InputManager;
    };
    inline static GLFWwindow* Window;
    inline static Mouse Mouse;
    static void Init(GLFWwindow* window_);
    //static void SetKeyCallback(keyfun callback);
    //static void SetMouseCursorCallback(cursorposfun callback);
    //static void SetMouseScrollCallback(scrollfun callback);
    //static void SetMouseButtonCallback(mousebuttonfun callback);
    static void SetKeyCallback(const std::function<void()>& callback);
    static void SetMouseCursorCallback(const std::function<void()>& callback);
    static void SetMouseScrollCallback(const std::function<void()>& callback);
    static void SetMouseButtonCallback(const std::function<void()>& callback);
    static void PollEvents();
};