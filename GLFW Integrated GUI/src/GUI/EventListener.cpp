#include "EventListener.h"

//#include <GLFW/glfw3.h>

#include "GUICanvas.h"
#include "GUISystem.h"


EventListener::EventListener(GUICanvas* element_,const std::function<bool(EventListener*)>& check,const std::function<void(EventListener*)>& onActivate):
	Check_(check), OnActivate_(onActivate), guiElement(element_) {}

EventListener::EventListener(GUICanvas* element_,const std::function<bool(EventListener*)>& check,const std::function<void(EventListener*)>& onActivate,const std::function<void()>& callback_):
	Check_(check),OnActivate_(onActivate), callback(callback_), guiElement(nullptr) {}

bool EventListener::Check() {
	return Check_(this);
}

void EventListener::OnActivate() {
	OnActivate_(this);
	callback();
}


//AAAA why do if i use them these way, they dont work!?!??/1/1?!?1?1??!/11?!?!
//inline std::function<bool(EventListener*)> EventListener::Check_SingleClick_Func =
//[](EventListener* el)->bool {
//	GLFWwindow* window = el->guiElement->guiSystem->window;
//
//	static bool clickHolding = false; //to not trigger every tick on hold
//
//	double mouseX;
//	double mouseY;
//	if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
//		glfwGetCursorPos(window,&mouseX,&mouseY);
//
//		if(clickHolding == true) {
//			return false;
//		}
//		clickHolding = true;
//
//		if(el->guiElement->CheckClick(mouseX,mouseY)){
//			return true;
//		}
//	} else {
//		clickHolding = false;
//	}
//	return false;
//};
//inline std::function<bool(EventListener*)> EventListener::Check_Hold_Func =
//[](EventListener* el)->bool {
//	 GLFWwindow* window = el->guiElement->guiSystem->window;
//
//	 double mouseX;
//	 double mouseY;
//	 if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
//		 glfwGetCursorPos(window,&mouseX,&mouseY);
//
//		 if(el->guiElement->CheckClick(mouseX,mouseY)){
//			 return true;
//		 }
//	 }
//	 return false;
// };

