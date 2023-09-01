#include "GUIGenericInteractable.h"

#include "../EventListener.h"
#include "../../InputManager.h"

void GUIGenericInteractable::setOnActivateListenerByLabel(const std::string& label_,const GUIEventSystem::EventType event,const std::function<void(GUIObject*)>& func) {
	for(const auto listener: listeners) {
		if(listener->label == label_) {
			listener->SetOnActivate(func);
			return;
		}
	}
	AddListener(new EventListener(
		label_,
		this,
		EL_Check_OnHover_Func,
		func
	));
	GUIEventSystem::DeSubscribeEvent(listeners.back(),GUIEventSystem::Always);
	GUIEventSystem::SubscribeEvent(listeners.back(),event);
}
void GUIGenericInteractable::setOnNotActivateListenerByLabel(const std::string& label_,const GUIEventSystem::EventType event,const std::function<void(GUIObject*)>& func) {
	for(const auto listener: listeners) {
		if(listener->label == label_) {
			listener->SetOnNotActivate(func);
			return;
		}
	}
	AddListener(new EventListener(
		label_,
		this,
		EL_Check_OnHover_Func,
		[](GUIObject*){},
		func
	));
	GUIEventSystem::DeSubscribeEvent(listeners.back(),GUIEventSystem::Always);
	GUIEventSystem::SubscribeEvent(listeners.back(),event);
}

void GUIGenericInteractable::SetOnMouseDown(const std::function<void(GUIObject*)>&func) {
	setOnActivateListenerByLabel("OnMouseDown",GUIEventSystem::EventType::MouseDown,func);
}
void GUIGenericInteractable::SetOnLeftMouseDown(const std::function<void(GUIObject*)>&func) {
	setOnActivateListenerByLabel("OnMouseLeftDown",GUIEventSystem::LeftMouseDown,func);
}
void GUIGenericInteractable::SetOnRightMouseDown(const std::function<void(GUIObject*)>&func) {
	setOnActivateListenerByLabel("OnMouseRightDown",GUIEventSystem::RightMouseDown,func);
}
void GUIGenericInteractable::SetOnMouseDownElse(const std::function<void(GUIObject*)>&func) {
	setOnNotActivateListenerByLabel("OnMouseDown",GUIEventSystem::MouseDown,func);
}
void GUIGenericInteractable::SetOnLeftMouseDownElse(const std::function<void(GUIObject*)>&func) {
	setOnNotActivateListenerByLabel("OnMouseLeftDown",GUIEventSystem::LeftMouseDown,func);
}
void GUIGenericInteractable::SetOnRightMouseDownElse(const std::function<void(GUIObject*)>&func) {
	setOnNotActivateListenerByLabel("OnMouseRightDown",GUIEventSystem::RightMouseDown,func);
}

void GUIGenericInteractable::SetOnMouseUp(const std::function<void(GUIObject*)>&func) {
	setOnActivateListenerByLabel("OnMouseUp",GUIEventSystem::MouseUp,func);
}
void GUIGenericInteractable::SetOnLeftMouseUp(const std::function<void(GUIObject*)>&func) {
	setOnActivateListenerByLabel("OnMouseLeftUp",GUIEventSystem::LeftMouseUp,func);
}
void GUIGenericInteractable::SetOnRightMouseUp(const std::function<void(GUIObject*)>&func) {
	setOnActivateListenerByLabel("OnMouseRightUp",GUIEventSystem::RightMouseUp,func);
}
void GUIGenericInteractable::SetOnMouseUpElse(const std::function<void(GUIObject*)>&func) {
	setOnNotActivateListenerByLabel("OnMouseUp",GUIEventSystem::MouseUp,func);
}
void GUIGenericInteractable::SetOnLeftMouseUpElse(const std::function<void(GUIObject*)>&func) {
	setOnNotActivateListenerByLabel("OnMouseLeftUp",GUIEventSystem::LeftMouseUp,func);
}
void GUIGenericInteractable::SetOnRightMouseUpElse(const std::function<void(GUIObject*)>&func) {
	setOnNotActivateListenerByLabel("OnMouseRightUp",GUIEventSystem::RightMouseUp,func);
}

void GUIGenericInteractable::SetOnMouseHover(const std::function<void(GUIObject*)>&func) {
	setOnActivateListenerByLabel("OnHover",GUIEventSystem::EventType::Always,func);
}
void GUIGenericInteractable::SetOnMouseHoverElse(const std::function<void(GUIObject*)>&func) {
	setOnNotActivateListenerByLabel("OnHover",GUIEventSystem::EventType::Always,func);
}
