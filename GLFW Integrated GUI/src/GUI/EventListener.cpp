#include "EventListener.h"

#include "GUICanvas.h"
#include "GUISystem.h"


EventListener::EventListener(GUICanvas* element_,const std::function<bool(EventListener*)>& check,const std::function<void(EventListener*)>& onActivate):
	Check_(check), OnActivate_(onActivate), guiElement(element_) {}
EventListener::EventListener(GUICanvas* element_,const std::function<bool(EventListener*)>& check,const std::function<void(EventListener*)>& onActivate,const std::function<void()>& callback_):
	Check_(check),OnActivate_(onActivate), callback(callback_), guiElement(element_) {}

EventListener::EventListener(const std::function<bool(EventListener*)>& check,const std::function<void(EventListener*)>& onActivate):
	Check_(check),OnActivate_(onActivate),guiElement(nullptr) {}
EventListener::EventListener(const std::function<bool(EventListener*)>& check,const std::function<void(EventListener*)>& onActivate,const std::function<void()>& callback_):
	Check_(check),OnActivate_(onActivate),callback(callback_),guiElement(nullptr) {}

bool EventListener::Check() {
	return Check_(this);
}

void EventListener::OnActivate() {
	OnActivate_(this);
	callback();
}

