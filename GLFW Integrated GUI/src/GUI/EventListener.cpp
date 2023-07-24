#include "EventListener.h"

#include "GUICanvas.h"
#include "GUISystem.h"


EventListener::EventListener(GUICanvas* element_,const std::function<bool(EventListener*)>& check,const std::function<void(EventListener*)>& onActivate):
	Check_(check), OnActivate_(onActivate),OnNotActive_([](EventListener* el){}),guiElement(element_) {}
EventListener::EventListener(GUICanvas* element_,const std::function<bool(EventListener*)>& check,const std::function<void(EventListener*)>& onActivate,const std::function<void()>& callback):
	Check_(check),OnActivate_(onActivate),OnNotActive_([](EventListener* el){}), Callback(callback), guiElement(element_) {}
EventListener::EventListener(GUICanvas* element_, const std::function<bool(EventListener*)>& check,const std::function<void(EventListener*)>& onActivate, const std::function<void(EventListener*)>& onNotActive,const std::function<void()>& callback):
	Check_(check),OnActivate_(onActivate),OnNotActive_(onNotActive),Callback(callback),guiElement(element_) {}

EventListener::EventListener(const std::function<bool(EventListener*)>& check,const std::function<void(EventListener*)>& onActivate):
	Check_(check),OnActivate_(onActivate),OnNotActive_( [](EventListener* el){}),guiElement(nullptr) {}
EventListener::EventListener(const std::function<bool(EventListener*)>& check,const std::function<void(EventListener*)>& onActivate,const std::function<void()>& callback):
	Check_(check),OnActivate_(onActivate),OnNotActive_([](EventListener* el){}),Callback(callback),guiElement(nullptr) {}
EventListener::EventListener(const std::function<bool(EventListener*)>& check,const std::function<void(EventListener*)>& onActivate, const std::function<void(EventListener*)>& onNotActive,const std::function<void()>& callback):
	Check_(check),OnActivate_(onActivate),OnNotActive_(onNotActive),Callback(callback),guiElement(nullptr) {}

bool EventListener::Check() {
	return Check_(this);
}

void EventListener::OnActivate() {
	OnActivate_(this);
	Callback();
}

void EventListener::OnNotActive() {
	OnNotActive_(this);
}

