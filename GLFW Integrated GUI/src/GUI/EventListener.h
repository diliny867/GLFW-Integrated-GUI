#pragma once

#include <iostream>
#include <functional>

class GUIObject;

class EventListener {
protected:
	std::function<bool(GUIObject*)> Check_;
	std::function<void(GUIObject*)> OnActivate_;
	std::function<void(GUIObject*)> OnNotActivate_;
	bool dirty = false;
public:
	std::string label;
	GUIObject* guiElement;
	EventListener(GUIObject* element_,const std::function<bool(GUIObject*)>& check,const std::function<void(GUIObject*)>& onActivate);
	EventListener(GUIObject* element_,const std::function<bool(GUIObject*)>& check,const std::function<void(GUIObject*)>& onActivate,const std::function<void(GUIObject*)>& onNotActive);
	EventListener(const std::function<bool(GUIObject*)>& check,const std::function<void(GUIObject*)>& onActivate);
	EventListener(const std::function<bool(GUIObject*)>& check,const std::function<void(GUIObject*)>& onActivate,const std::function<void(GUIObject*)>& onNotActive);

	EventListener(const std::string& label_,GUIObject* element_,const std::function<bool(GUIObject*)>& check,const std::function<void(GUIObject*)>& onActivate);
	EventListener(const std::string& label_,GUIObject* element_,const std::function<bool(GUIObject*)>& check,const std::function<void(GUIObject*)>& onActivate,const std::function<void(GUIObject*)>& onNotActive);
	EventListener(const std::string& label_,const std::function<bool(GUIObject*)>& check,const std::function<void(GUIObject*)>& onActivate);
	EventListener(const std::string& label_,const std::function<bool(GUIObject*)>& check,const std::function<void(GUIObject*)>& onActivate,const std::function<void(GUIObject*)>& onNotActive);

	/*
	EventListener(GUIObject* element_,const std::function<bool(GUIObject*)>& check,const std::function<void(GUIObject*)>& onActivate);
	EventListener(GUIObject* element_,const std::function<bool(GUIObject*)>& check,const std::function<void(GUIObject*)>& onActivate,const std::function<void(GUIObject*)>& onNotActive);
	EventListener(const std::function<bool(GUIObject*)>& check,const std::function<void(GUIObject*)>& onActivate);
	EventListener(const std::function<bool(GUIObject*)>& check,const std::function<void(GUIObject*)>& onActivate,const std::function<void(GUIObject*)>& onNotActive);

	EventListener(const std::string& label_,GUIObject* element_,const std::function<bool(GUIObject*)>& check,const std::function<void(GUIObject*)>& onActivate);
	EventListener(const std::string& label_,GUIObject* element_,const std::function<bool(GUIObject*)>& check,const std::function<void(GUIObject*)>& onActivate,const std::function<void(GUIObject*)>& onNotActive);
	EventListener(const std::string& label_,const std::function<bool(GUIObject*)>& check,const std::function<void(GUIObject*)>& onActivate);
	EventListener(const std::string& label_,const std::function<bool(GUIObject*)>& check,const std::function<void(GUIObject*)>& onActivate,const std::function<void(GUIObject*)>& onNotActive);

	EventListener(GUIObject* element_,const EventType type_,const std::function<bool(GUIObject*)>& check,const std::function<void(GUIObject*)>& onActivate);
	EventListener(GUIObject* element_,const EventType type_,const std::function<bool(GUIObject*)>& check,const std::function<void(GUIObject*)>& onActivate,const std::function<void(GUIObject*)>& onNotActive);
	EventListener(const EventType type_,const std::function<bool(GUIObject*)>& check,const std::function<void(GUIObject*)>& onActivate);
	EventListener(const EventType type_,const std::function<bool(GUIObject*)>& check,const std::function<void(GUIObject*)>& onActivate,const std::function<void(GUIObject*)>& onNotActive);

	EventListener(const std::string& label_,GUIObject* element_,const EventType type_,const std::function<bool(GUIObject*)>& check,const std::function<void(GUIObject*)>& onActivate);
	EventListener(const std::string& label_,GUIObject* element_,const EventType type_,const std::function<bool(GUIObject*)>& check,const std::function<void(GUIObject*)>& onActivate,const std::function<void(GUIObject*)>& onNotActive);
	EventListener(const std::string& label_,const EventType type_,const std::function<bool(GUIObject*)>& check,const std::function<void(GUIObject*)>& onActivate);
	EventListener(const std::string& label_,const EventType type_,const std::function<bool(GUIObject*)>& check,const std::function<void(GUIObject*)>& onActivate,const std::function<void(GUIObject*)>& onNotActive);
	 */

	void SetCheck(const std::function<bool(GUIObject*)>& check);
	void SetOnActivate(const std::function<void(GUIObject*)>& onActivate);
	void SetOnNotActivate(const std::function<void(GUIObject*)>& onNotActivate);

	void MarkDirty();
	bool IsDirty();
	void UnDirty();
	
	bool Check(); //If event listener check worked
	void OnActivate(); //To change internal state of object, event listener is bound to
	void OnNotActive(); //maybe remove this?
};


#define EL_Check_Always_Func												\
[](const GUIObject* obj)->bool {											\
	return true;															\
}
#define EL_Check_Never_Func													\
[](const GUIObject* obj)->bool {											\
	return false;															\
}
#define EL_Check_MouseDown_Func												\
[](const GUIObject* obj)->bool {											\
	const InputManager::Mouse& mouse = InputManager::mouse;					\
	static bool clickHolding = false;										\
	if(mouse.leftPress || mouse.rightPress) {								\
		if(clickHolding == true) {											\
			return false;													\
		}																	\
		clickHolding = true;												\
		if(obj->CheckClick(mouse.posX,mouse.posY)){							\
			return true;													\
		}																	\
	} else {																\
		clickHolding = false;												\
	}																		\
	return false;															\
}
#define EL_Check_MouseDownLMB_Func											\
[](const GUIObject* obj)->bool {											\
	const InputManager::Mouse& mouse = InputManager::mouse;					\
	static bool clickHolding = false;										\
	if(mouse.leftPress) {													\
		if(clickHolding == true) {											\
			return false;													\
		}																	\
		clickHolding = true;												\
		if(obj->CheckClick(mouse.posX,mouse.posY)){							\
			return true;													\
		}																	\
	} else {																\
		clickHolding = false;												\
	}																		\
	return false;															\
}
#define EL_Check_MouseDownRMB_Func											\
[](const GUIObject* obj)->bool {											\
	const InputManager::Mouse& mouse = InputManager::mouse;					\
	static bool clickHolding = false;										\
	if(mouse.rightPress) {													\
		if(clickHolding == true) {											\
			return false;													\
		}																	\
		clickHolding = true;												\
		if(obj->CheckClick(mouse.posX,mouse.posY)){							\
			return true;													\
		}																	\
	} else {																\
		clickHolding = false;												\
	}																		\
	return false;															\
}

#define EL_Check_Hold_Func													\
[](const GUIObject* obj)->bool {											\
	const InputManager::Mouse& mouse = InputManager::mouse;					\
	if(mouse.leftPress || mouse.rightPress) {								\
		if(obj->CheckClick(mouse.posX,mouse.posY)){							\
			return true;													\
		}																	\
	}																		\
	return false;															\
}
#define EL_Check_HoldLMB_Func												\
[](const GUIObject* obj)->bool {											\
	const InputManager::Mouse& mouse = InputManager::mouse;					\
	if(mouse.leftPress) {													\
		if(obj->CheckClick(mouse.posX,mouse.posY)){							\
			return true;													\
		}																	\
	}																		\
	return false;															\
}
#define EL_Check_HoldRMB_Func												\
[](const GUIObject* obj)->bool {											\
	const InputManager::Mouse& mouse = InputManager::mouse;					\
	if(mouse.rightPress) {													\
		if(obj->CheckClick(mouse.posX,mouse.posY)){							\
			return true;													\
		}																	\
	}																		\
	return false;															\
}

#define EL_Check_HoldUntilRelease_Func										\
[](const GUIObject* obj)->bool {											\
	const InputManager::Mouse& mouse = InputManager::mouse;					\
	static bool captured = false;											\
	if(mouse.leftPress || mouse.rightPress) {								\
		if(captured || obj->CheckClick(mouse.posX,mouse.posY)){				\
			captured = true;												\
			return true;													\
		}																	\
	}else {																	\
		captured = false;													\
	}																		\
	return false;															\
}
#define EL_Check_HoldUntilReleaseLMB_Func									\
[](const GUIObject* obj)->bool {											\
	const InputManager::Mouse& mouse = InputManager::mouse;					\
	static bool captured = false;											\
	if(mouse.leftPress) {													\
		if(captured || obj->CheckClick(mouse.posX,mouse.posY)){				\
			captured = true;												\
			return true;													\
		}																	\
	}else {																	\
		captured = false;													\
	}																		\
	return false;															\
}
#define EL_Check_HoldUntilReleaseRMB_Func									\
[](const GUIObject* obj)->bool {											\
	const InputManager::Mouse& mouse = InputManager::mouse;					\
	static bool captured = false;											\
	if(mouse.rightPress) {													\
		if(captured || obj->CheckClick(mouse.posX,mouse.posY)){				\
			captured = true;												\
			return true;													\
		}																	\
	} else {																\
		captured = false;													\
	}																		\
	return false;															\
}

#define EL_Check_OnHover_Func												\
[](const GUIObject* obj)->bool {											\
	const InputManager::Mouse& mouse = InputManager::mouse;					\
	return obj->CheckClick(mouse.posX,mouse.posY);							\
}
#define EL_Check_NotHover_Func												\
[](const GUIObject* obj)->bool {											\
	const InputManager::Mouse& mouse = InputManager::mouse;					\
	return !obj->CheckClick(mouse.posX,mouse.posY);							\
}

#define EL_Check_StartHereHold_Func											\
[](const GUIObject* obj)->bool {											\
	const InputManager::Mouse& mouse = InputManager::mouse;					\
	static bool captured = false;											\
	static bool clickHolding = false;										\
	if(mouse.leftPress || mouse.rightPress) {								\
		if(!clickHolding &&													\
			obj->CheckClick(mouse.posX,mouse.posY)){						\
			captured = true;												\
		}																	\
		clickHolding = true;												\
		if(captured){														\
			return obj->CheckClick(mouse.posX,mouse.posY);					\
		}																	\
	}else {																	\
		captured = false;													\
		clickHolding = false;												\
	}																		\
	return false;															\
}
#define EL_Check_StartHereHoldLMB_Func										\
[](const GUIObject* obj)->bool {											\
	const InputManager::Mouse& mouse = InputManager::mouse;					\
	static bool captured = false;											\
	static bool clickHolding = false;										\
	if(mouse.leftPress) {													\
		if(!clickHolding &&													\
			obj->CheckClick(mouse.posX,mouse.posY)){						\
			captured = true;												\
		}																	\
		clickHolding = true;												\
		if(captured){														\
			return obj->CheckClick(mouse.posX,mouse.posY);					\
		}																	\
	}else {																	\
		captured = false;													\
		clickHolding = false;												\
	}																		\
	return false;															\
}
#define EL_Check_StartHereHoldRMB_Func										\
[](const GUIObject* obj)->bool {											\
	const InputManager::Mouse& mouse = InputManager::mouse;					\
	static bool captured = false;											\
	static bool clickHolding = false;										\
	if(mouse.rightPress) {													\
		if(!clickHolding &&													\
			obj->CheckClick(mouse.posX,mouse.posY)){						\
			captured = true;												\
		}																	\
		clickHolding = true;												\
		if(captured){														\
			return obj->CheckClick(mouse.posX,mouse.posY);					\
		}																	\
	}else {																	\
		captured = false;													\
		clickHolding = false;												\
	}																		\
	return false;															\
}

#define EL_Check_StartHereHoldUntilRelease_Func								\
[](const GUIObject* obj)->bool {											\
	const InputManager::Mouse& mouse = InputManager::mouse;					\
	static bool captured = false;											\
	static bool clickHolding = false;										\
	if(mouse.leftPress || mouse.rightPress) {								\
		if(!clickHolding &&													\
			obj->CheckClick(mouse.posX,mouse.posY)){						\
			captured = true;												\
		}																	\
		clickHolding = true;												\
		if(captured){														\
			return true;													\
		}																	\
	}else {																	\
		captured = false;													\
		clickHolding = false;												\
	}																		\
	return false;															\
}
#define EL_Check_StartHereHoldUntilReleaseLMB_Func							\
[](const GUIObject* obj)->bool {											\
	const InputManager::Mouse& mouse = InputManager::mouse;					\
	static bool captured = false;											\
	static bool clickHolding = false;										\
	if(mouse.leftPress) {													\
		if(!clickHolding &&													\
			obj->CheckClick(mouse.posX,mouse.posY)){						\
			captured = true;												\
		}																	\
		clickHolding = true;												\
		if(captured){														\
			return true;													\
		}																	\
	}else {																	\
		captured = false;													\
		clickHolding = false;												\
	}																		\
	return false;															\
}
#define EL_Check_StartHereHoldUntilReleaseRMB_Func							\
[](const GUIObject* obj)->bool {											\
	const InputManager::Mouse& mouse = InputManager::mouse;					\
	static bool captured = false;											\
	static bool clickHolding = false;										\
	if(mouse.rightPress) {													\
		if(!clickHolding &&													\
			obj->CheckClick(mouse.posX,mouse.posY)){						\
			captured = true;												\
		}																	\
		clickHolding = true;												\
		if(captured){														\
			return true;													\
		}																	\
	}else {																	\
		captured = false;													\
		clickHolding = false;												\
	}																		\
	return false;															\
}