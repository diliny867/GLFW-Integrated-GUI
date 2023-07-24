#pragma once

#include <iostream>
#include <functional>

class GUICanvas;

class EventListener {
protected:
	//using CheckFuncType = bool(EventListener*);
	//using OnActivateFuncType = void(EventListener*);
	//CheckFuncType* Check_;
	//OnActivateFuncType* OnActivate_;
	std::function<bool(EventListener*)> Check_;
	std::function<void(EventListener*)> OnActivate_;
	std::function<void(EventListener*)> OnNotActive_;
	std::function<void()> callback;
public:
	GUICanvas* guiElement;
	//EventListener(GUICanvas* element_,CheckFuncType* check,OnActivateFuncType* onActivate);
	//EventListener(GUICanvas* element_,CheckFuncType* check,OnActivateFuncType* onActivate,const std::function<void()>& callback_);
	EventListener(GUICanvas* element_,const std::function<bool(EventListener*)>& check,const std::function<void(EventListener*)>& onActivate);
	EventListener(GUICanvas* element_,const std::function<bool(EventListener*)>& check,const std::function<void(EventListener*)>& onActivate,const std::function<void()>& callback_);
	EventListener(GUICanvas* element_,const std::function<bool(EventListener*)>& check,const std::function<void(EventListener*)>& onActivate,const std::function<void(EventListener*)>& onNotActive,const std::function<void()>& callback_);
	EventListener(const std::function<bool(EventListener*)>& check,const std::function<void(EventListener*)>& onActivate);
	EventListener(const std::function<bool(EventListener*)>& check,const std::function<void(EventListener*)>& onActivate,const std::function<void()>& callback_);
	EventListener(const std::function<bool(EventListener*)>& check,const std::function<void(EventListener*)>& onActivate,const std::function<void(EventListener*)>& onNotActive,const std::function<void()>& callback_);

	bool Check(); //If event listener check worked
	void OnActivate(); //To change internal state of object, event listener is bound to
	void OnNotActive();
};



#define EL_Check_SingleClick_Func											\
[](const EventListener* el)->bool {											\
	const InputManager::Mouse& mouse = InputManager::mouse;					\
	static bool clickHolding = false;										\
	if(mouse.leftPress) {													\
		if(clickHolding == true) {											\
			return false;													\
		}																	\
		clickHolding = true;												\
		if(el->guiElement->CheckClick(mouse.posX,mouse.posY)){				\
			return true;													\
		}																	\
	} else {																\
		clickHolding = false;												\
	}																		\
	return false;															\
}
#define EL_Check_Hold_Func													\
[](const EventListener* el)->bool {											\
	const InputManager::Mouse& mouse = InputManager::mouse;					\
	if(mouse.leftPress) {													\
		if(el->guiElement->CheckClick(mouse.posX,mouse.posY)){				\
			return true;													\
		}																	\
	}																		\
	return false;															\
}
#define EL_Check_HoldUntilRelease_Func										\
[](const EventListener* el)->bool {											\
	const InputManager::Mouse& mouse = InputManager::mouse;					\
	static bool captured = false;											\
	if(mouse.leftPress) {													\
		if(captured || el->guiElement->CheckClick(mouse.posX,mouse.posY)){	\
			captured = true;												\
			return true;													\
		}																	\
	}else {																	\
		captured = false;													\
	}																		\
	return false;															\
}
#define EL_Check_SingleClickRMB_Func										\
[](const EventListener* el)->bool {											\
	const InputManager::Mouse& mouse = InputManager::mouse;					\
	static bool clickHolding = false;										\
	if(mouse.rightPress) {													\
		if(clickHolding == true) {											\
			return false;													\
		}																	\
		clickHolding = true;												\
		if(el->guiElement->CheckClick(mouse.posX,mouse.posY)){				\
			return true;													\
		}																	\
	} else {																\
		clickHolding = false;												\
	}																		\
	return false;															\
}
#define EL_Check_HoldRMB_Func												\
[](const EventListener* el)->bool {											\
	const InputManager::Mouse& mouse = InputManager::mouse;					\
	if(mouse.rightPress) {													\
		if(el->guiElement->CheckClick(mouse.posX,mouse.posY)){				\
			return true;													\
		}																	\
	}																		\
	return false;															\
}
#define EL_Check_HoldUntilReleaseRMB_Func									\
[](const EventListener* el)->bool {											\
	const InputManager::Mouse& mouse = InputManager::mouse;					\
	static bool captured = false;											\
	if(mouse.rightPress) {													\
		if(captured || el->guiElement->CheckClick(mouse.posX,mouse.posY)){	\
			captured = true;												\
			return true;													\
		}																	\
	} else {																\
		captured = false;													\
	}																		\
	return false;															\
}
#define EL_Check_OnHover_Func												\
[](const EventListener* el)->bool {											\
	const InputManager::Mouse& mouse = InputManager::mouse;					\
	return el->guiElement->CheckClick(mouse.posX,mouse.posY);				\
}
#define EL_Check_NotHover_Func												\
[](const EventListener* el)->bool {											\
	const InputManager::Mouse& mouse = InputManager::mouse;					\
	return !el->guiElement->CheckClick(mouse.posX,mouse.posY);				\
}