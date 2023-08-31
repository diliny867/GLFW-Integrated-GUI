#include "GUIGenericInteractable.h"

#include "../EventListener.h"
#include "../../InputManager.h"

void GUIGenericInteractable::setOnActivateListenerByLabel(const std::string&label_,const std::function<bool(GUIObject*)>&check,const std::function<void(GUIObject*)>&func) {
	for(const auto listener: listeners) {
		if(listener->label == label_) {
			listener->SetOnActivate(func);
			return;
		}
	}
	AddListener(new EventListener(
		label_,
		this,
		check,
		func
	));
}
void GUIGenericInteractable::setOnNotActivateListenerByLabel(const std::string&label_,const std::function<bool(GUIObject*)>&check,const std::function<void(GUIObject*)>&func) {
	for(const auto listener: listeners) {
		if(listener->label == label_) {
			listener->SetOnNotActivate(func);
			return;
		}
	}
	AddListener(new EventListener(
		label_,
		this,
		check,
		[](GUIObject*){},
		func
	));
}

void GUIGenericInteractable::SetOnMouseDown(const std::function<void(GUIObject*)>&func) {
	setOnActivateListenerByLabel("OnMouseClick",EL_Check_MouseDown_Func,func);
}
void GUIGenericInteractable::SetOnLeftMouseDown(const std::function<void(GUIObject*)>&func) {
	setOnActivateListenerByLabel("OnMouseLeftClick",EL_Check_MouseDownLMB_Func,func);
}
void GUIGenericInteractable::SetOnRightMouseDown(const std::function<void(GUIObject*)>&func) {
	setOnActivateListenerByLabel("OnMouseRightClick",EL_Check_MouseDownRMB_Func,func);
}
void GUIGenericInteractable::SetOnMouseDownElse(const std::function<void(GUIObject*)>&func) {
	setOnNotActivateListenerByLabel("OnMouseClick",EL_Check_MouseDown_Func,func);
}
void GUIGenericInteractable::SetOnLeftMouseDownElse(const std::function<void(GUIObject*)>&func) {
	setOnNotActivateListenerByLabel("OnMouseLeftClick",EL_Check_MouseDownLMB_Func,func);
}
void GUIGenericInteractable::SetOnRightMouseDownElse(const std::function<void(GUIObject*)>&func) {
	setOnNotActivateListenerByLabel("OnMouseRightClick",EL_Check_MouseDownRMB_Func,func);
}

void GUIGenericInteractable::SetOnMouseHover(const std::function<void(GUIObject*)>&func) {
	setOnActivateListenerByLabel("OnHover",EL_Check_OnHover_Func,func);
}
void GUIGenericInteractable::SetOnMouseHoverElse(const std::function<void(GUIObject*)>&func) {
	setOnNotActivateListenerByLabel("OnHover",EL_Check_OnHover_Func,func);
}
