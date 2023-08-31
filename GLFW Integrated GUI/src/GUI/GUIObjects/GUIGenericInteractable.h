#pragma once

#include <functional>
#include <string>

#include "../GUIObject.h"

class GUIGenericInteractable: public GUIObject {
private:
	void setOnActivateListenerByLabel(const std::string& label_,const std::function<bool(GUIObject*)>& check,const std::function<void(GUIObject*)>& func);
	void setOnNotActivateListenerByLabel(const std::string& label_,const std::function<bool(GUIObject*)>& check,const std::function<void(GUIObject*)>& func);
protected:
	void SetOnMouseDown(const std::function<void(GUIObject*)>& func);
	void SetOnLeftMouseDown(const std::function<void(GUIObject*)>& func);
	void SetOnRightMouseDown(const std::function<void(GUIObject*)>& func);
	void SetOnMouseDownElse(const std::function<void(GUIObject*)>& func);
	void SetOnLeftMouseDownElse(const std::function<void(GUIObject*)>& func);
	void SetOnRightMouseDownElse(const std::function<void(GUIObject*)>& func);

	void SetOnMouseHover(const std::function<void(GUIObject*)>& func);
	void SetOnMouseHoverElse(const std::function<void(GUIObject*)>& func);
public:
	using GUIObject::GUIObject;
};