#include "GUIEventSystem.h"


#include "../InputManager.h"

#include "GUIObject.h"
#include "EventListener.h"


//void GUIEventSystem::SubscribeMouseDown(EventListener* el) {
//	listeners.at(EventType::MouseDown).push_back(el);
//}
//void GUIEventSystem::SubscribeLeftMouseDown(EventListener* el) {
//
//	listeners.at(EventType::LeftMouseDown).push_back(el);
//}
//void GUIEventSystem::SubscribeRightMouseDown(EventListener* el) {
//	listeners.at(EventType::RightMouseDown).push_back(el);
//}
//
//void GUIEventSystem::SubscribeMouseUp(EventListener* el) {
//	listeners.at(EventType::MouseUp).push_back(el);
//}
//void GUIEventSystem::SubscribeLeftMouseUp(EventListener* el) {
//	listeners.at(EventType::LeftMouseUp).push_back(el);
//}
//void GUIEventSystem::SubscribeRightMouseUp(EventListener* el) {
//	listeners.at(EventType::RightMouseUp).push_back(el);
//}
//
//void GUIEventSystem::SubscribeMouseHold(EventListener* el) {
//	listeners.at(EventType::MouseHold).push_back(el);
//}
//void GUIEventSystem::SubscribeLeftMouseHold(EventListener*el) {
//	listeners.at(EventType::LeftMouseHold).push_back(el);
//}
//void GUIEventSystem::SubscribeRightMouseHold(EventListener*el) {
//	listeners.at(EventType::RightMouseHold).push_back(el);
//}
//
//void GUIEventSystem::SubscribeAlways(EventListener*el) {
//	listeners.at(EventType::Always).push_back(el);
//}

void GUIEventSystem::SubscribeEvent(EventListener* el,const EventType event) {
	listeners.try_emplace(event,std::vector<EventListener*>());
	listeners.at(event).push_back(el);
}
void GUIEventSystem::DeSubscribeEvent(EventListener* el,const EventType event) {
	auto& listenersVec = listeners.at(event);
	for(auto it = listenersVec.begin();it<listenersVec.end();) {
		if(*it == el) {
			it = listenersVec.erase(it);
			return;
		}else {
			++it;
		}
	}
}
void GUIEventSystem::DeSubscribeAllEvents(EventListener* el,const EventType event) {
	auto& listenersVec = listeners.at(event);
	for(auto it = listenersVec.begin();it<listenersVec.end();) {
		if(*it == el) {
			it = listenersVec.erase(it);
		}else {
			++it;
		}
	}
}

void GUIEventSystem::NotifyByEventType(const EventType type) {

	for(const auto listener: listeners.at(type)) {
		//listener->guiElement->listenersToBeChecked.push(listener);
		listener->MarkDirty();
	}
}

void GUIEventSystem::NotifyMouseDown() {
	NotifyByEventType(EventType::MouseDown);
	NotifyByEventType(EventType::LeftMouseDown);
	NotifyByEventType(EventType::RightMouseDown);
}
void GUIEventSystem::NotifyMouseHold() {
	NotifyByEventType(EventType::MouseHold);
	NotifyByEventType(EventType::LeftMouseHold);
	NotifyByEventType(EventType::RightMouseHold);
}
void GUIEventSystem::NotifyMouseUp() {
	NotifyByEventType(EventType::MouseUp);
	NotifyByEventType(EventType::LeftMouseUp);
	NotifyByEventType(EventType::RightMouseUp);
}

void GUIEventSystem::Init() {
	for(int i=0;i<=EventType::RightMouseHold;i++) {
		listeners[EventType(i)] = std::vector<EventListener*>();
	}
}

void GUIEventSystem::TryNotifySubscribers() {
	const InputManager::Mouse& mouse = InputManager::mouse;
	if(lastLeftMouse == false && mouse.leftPress == true || lastRightMouse == false && mouse.rightPress == true){
		NotifyMouseDown();
	}
	if(mouse.leftPress || mouse.rightPress){
		NotifyMouseHold();
	}
	if(lastLeftMouse == true && mouse.leftPress == false || lastRightMouse == true && mouse.rightPress == false){
		NotifyMouseUp();
	}
	NotifyByEventType(EventType::Always);
	lastLeftMouse = mouse.leftPress;
	lastRightMouse = mouse.rightPress;
}
