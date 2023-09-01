#pragma once

#include <functional>
#include <string>

#include "../EventListener.h"
#include "../GUIObject.h"
#include "../GUIEventSystem.h"

class GUIGenericInteractable: public GUIObject {
protected:
	void setOnActivateListenerByLabel(const std::string& label_,const GUIEventSystem::EventType event,const std::function<void(GUIObject*)>& func);
	void setOnNotActivateListenerByLabel(const std::string& label_,const GUIEventSystem::EventType event,const std::function<void(GUIObject*)>& func);
protected:
	void SetOnMouseDown(const std::function<void(GUIObject*)>& func);
	void SetOnLeftMouseDown(const std::function<void(GUIObject*)>& func);
	void SetOnRightMouseDown(const std::function<void(GUIObject*)>& func);
	void SetOnMouseDownElse(const std::function<void(GUIObject*)>& func);
	void SetOnLeftMouseDownElse(const std::function<void(GUIObject*)>& func);
	void SetOnRightMouseDownElse(const std::function<void(GUIObject*)>& func);

	void SetOnMouseUp(const std::function<void(GUIObject*)>& func);
	void SetOnLeftMouseUp(const std::function<void(GUIObject*)>& func);
	void SetOnRightMouseUp(const std::function<void(GUIObject*)>& func);
	void SetOnMouseUpElse(const std::function<void(GUIObject*)>& func);
	void SetOnLeftMouseUpElse(const std::function<void(GUIObject*)>& func);
	void SetOnRightMouseUpElse(const std::function<void(GUIObject*)>& func);

	void SetOnMouseHover(const std::function<void(GUIObject*)>& func);
	void SetOnMouseHoverElse(const std::function<void(GUIObject*)>& func);
public:
	using GUIObject::GUIObject;
};