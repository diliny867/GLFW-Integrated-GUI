#include "EventListener.h"

#include "GUIObject.h"
#include "GUISystem.h"


class GUIObject;

EventListener::EventListener(GUIObject* element_,const std::function<bool(GUIObject*)>& check,const std::function<void(GUIObject*)>& onActivate):
	EventListener("",element_,check,onActivate,[](GUIObject* el){}) {}
EventListener::EventListener(GUIObject* element_,const std::function<bool(GUIObject*)>& check,const std::function<void(GUIObject*)>& onActivate,const std::function<void(GUIObject*)>& onNotActive):
	EventListener("",element_,check,onActivate,[](GUIObject* el){}) {}
EventListener::EventListener(const std::function<bool(GUIObject*)>& check,const std::function<void(GUIObject*)>& onActivate):
	EventListener("",nullptr,check,onActivate,[](GUIObject* el){}) {}
EventListener::EventListener(const std::function<bool(GUIObject*)>& check,const std::function<void(GUIObject*)>& onActivate,const std::function<void(GUIObject*)>& onNotActive):
	EventListener("",nullptr,check,onActivate,onNotActive) {}

EventListener::EventListener(const std::string& label_,GUIObject* element_,const std::function<bool(GUIObject*)>& check,const std::function<void(GUIObject*)>& onActivate):
	EventListener(label_,element_,check,onActivate,[](GUIObject* el){}) {}
EventListener::EventListener(const std::string& label_,GUIObject* element_,const std::function<bool(GUIObject*)>& check,const std::function<void(GUIObject*)>& onActivate,const std::function<void(GUIObject*)>& onNotActive):
	Check_(check),OnActivate_(onActivate),OnNotActivate_(onNotActive),label(label_),guiElement(element_) {
	GUIEventSystem::SubscribeEvent(this,GUIEventSystem::Always);
}
EventListener::EventListener(const std::string& label_,const std::function<bool(GUIObject*)>& check,const std::function<void(GUIObject*)>& onActivate):
	EventListener(label_,nullptr,check,onActivate,[](GUIObject* el){}) {}
EventListener::EventListener(const std::string& label_,const std::function<bool(GUIObject*)>& check,const std::function<void(GUIObject*)>& onActivate,const std::function<void(GUIObject*)>& onNotActive):
	EventListener(label_,nullptr,check,onActivate,onNotActive) {}

/*
EventListener::EventListener(GUIObject* element_,const std::function<bool(GUIObject*)>& check,const std::function<void(GUIObject*)>& onActivate):
	EventListener("",element_,EventType::Custom,check,onActivate,[](GUIObject* el){}) {}
EventListener::EventListener(GUIObject* element_,const std::function<bool(GUIObject*)>& check,const std::function<void(GUIObject*)>& onActivate,const std::function<void(GUIObject*)>& onNotActive):
	EventListener("",element_,EventType::Custom,check,onActivate,[](GUIObject* el){}) {}
EventListener::EventListener(const std::function<bool(GUIObject*)>& check,const std::function<void(GUIObject*)>& onActivate):
	EventListener("",nullptr,EventType::Custom,check,onActivate,[](GUIObject* el){}) {}
EventListener::EventListener(const std::function<bool(GUIObject*)>& check,const std::function<void(GUIObject*)>& onActivate,const std::function<void(GUIObject*)>& onNotActive):
	EventListener("",nullptr,EventType::Custom,check,onActivate,onNotActive) {}

EventListener::EventListener(const std::string& label_,GUIObject* element_,const std::function<bool(GUIObject*)>& check,const std::function<void(GUIObject*)>& onActivate):
	EventListener(label_,element_,EventType::Custom,check,onActivate,[](GUIObject* el){}) {}
EventListener::EventListener(const std::string& label_,GUIObject* element_,const std::function<bool(GUIObject*)>& check,const std::function<void(GUIObject*)>& onActivate,const std::function<void(GUIObject*)>& onNotActive):
	EventListener(label_,element_,EventType::Custom,check,onActivate,onNotActive) {}
EventListener::EventListener(const std::string& label_,const std::function<bool(GUIObject*)>& check,const std::function<void(GUIObject*)>& onActivate):
	EventListener(label_,nullptr,EventType::Custom,check,onActivate,[](GUIObject* el){}) {}
EventListener::EventListener(const std::string& label_,const std::function<bool(GUIObject*)>& check,const std::function<void(GUIObject*)>& onActivate,const std::function<void(GUIObject*)>& onNotActive):
	EventListener(label_,nullptr,EventType::Custom,check,onActivate,onNotActive) {}

EventListener::EventListener(GUIObject*element_,const EventType type_,const std::function<bool(GUIObject*)>&check,const std::function<void(GUIObject*)>&onActivate):
	EventListener("",element_,type_,check,onActivate,[](GUIObject* el){}) {}
EventListener::EventListener(GUIObject*element_,const EventType type_,const std::function<bool(GUIObject*)>&check,const std::function<void(GUIObject*)>&onActivate,const std::function<void(GUIObject*)>&onNotActive):
	EventListener("",element_,type_,check,onActivate,[](GUIObject* el){}){}
EventListener::EventListener(const EventType type_,const std::function<bool(GUIObject*)>&check,const std::function<void(GUIObject*)>&onActivate):
	EventListener("",nullptr,type_,check,onActivate,[](GUIObject* el){}) {}
EventListener::EventListener(const EventType type_,const std::function<bool(GUIObject*)>&check,const std::function<void(GUIObject*)>&onActivate,const std::function<void(GUIObject*)>&onNotActive):
	EventListener("",nullptr,type_,check,onActivate,onNotActive) {}

EventListener::EventListener(const std::string&label_,GUIObject*element_,const EventType type_,const std::function<bool(GUIObject*)>&check,const std::function<void(GUIObject*)>&onActivate):
	EventListener(label_,element_,type_,check,onActivate,[](GUIObject* el){}) {}
EventListener::EventListener(const std::string&label_,GUIObject*element_,const EventType type_,const std::function<bool(GUIObject*)>&check,const std::function<void(GUIObject*)>&onActivate,const std::function<void(GUIObject*)>&onNotActive):
	Check_(check),OnActivate_(onActivate),OnNotActivate_(onNotActive),label(label_),type(type_),guiElement(element_) {}
EventListener::EventListener(const std::string&label_,const EventType type_,const std::function<bool(GUIObject*)>&check,const std::function<void(GUIObject*)>&onActivate):
	EventListener(label_,nullptr,type_,check,onActivate,[](GUIObject* el){}) {}
EventListener::EventListener(const std::string&label_,const EventType type_,const std::function<bool(GUIObject*)>&check,const std::function<void(GUIObject*)>&onActivate,const std::function<void(GUIObject*)>&onNotActive):
	EventListener(label_,nullptr,type_,check,onActivate,onNotActive) {}
 */

void EventListener::SetCheck(const std::function<bool(GUIObject*)>& check) {
	Check_ = check;
}
void EventListener::SetOnActivate(const std::function<void(GUIObject*)>& onActivate) {
	OnActivate_ = onActivate;
}
void EventListener::SetOnNotActivate(const std::function<void(GUIObject*)>& onNotActivate) {
	OnNotActivate_ = onNotActivate;
}

void EventListener::MarkDirty() {
	dirty = true;
}
bool EventListener::IsDirty() {
	return dirty;
}
void EventListener::UnDirty() {
	dirty = false;
}

bool EventListener::Check() {
	return Check_(this->guiElement);
}

void EventListener::OnActivate() {
	OnActivate_(this->guiElement);
}

void EventListener::OnNotActive() {
	OnNotActivate_(this->guiElement);
}

