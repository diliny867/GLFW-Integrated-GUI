#include "EventListener.h"

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

std::function<bool(EventListener*)> EventListener::Check_SingleClick_Func =
[](EventListener* el)->bool {

	static bool clickHolding = false; //to not trigger every tick on hold

	if(InputManager::Mouse.leftPress) {

		if(clickHolding == true) {
			return false;
		}
		clickHolding = true;

		if(el->guiElement->CheckClick(InputManager::Mouse.posX,InputManager::Mouse.posY)){
			return true;
		}
	} else {
		clickHolding = false;
	}
	return false;
};
std::function<bool(EventListener*)> EventListener::Check_Hold_Func =
[](EventListener* el)->bool {

	if(InputManager::Mouse.leftPress) {

		if(el->guiElement->CheckClick(InputManager::Mouse.posX,InputManager::Mouse.posY)){
			return true;
		}
	}
	return false;
};
std::function<bool(EventListener*)> EventListener::Check_HoldUntilRelease_Func =
[](EventListener* el)->bool {

	static bool captured = false;

	if(InputManager::Mouse.leftPress) {
		if(captured || el->guiElement->CheckClick(InputManager::Mouse.posX,InputManager::Mouse.posY)){
			captured = true;
			return true;
		}
	}else {
		captured = false;
	}
	return false;
};

