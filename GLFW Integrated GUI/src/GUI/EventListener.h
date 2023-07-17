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
	std::function<void()> callback;
public:
	GUICanvas* guiElement;
	//EventListener(GUICanvas* element_,CheckFuncType* check,OnActivateFuncType* onActivate);
	//EventListener(GUICanvas* element_,CheckFuncType* check,OnActivateFuncType* onActivate,const std::function<void()>& callback_);
	EventListener(GUICanvas* element_,const std::function<bool(EventListener*)>& check,const std::function<void(EventListener*)>& onActivate);
	EventListener(GUICanvas* element_,const std::function<bool(EventListener*)>& check,const std::function<void(EventListener*)>& onActivate,const std::function<void()>& callback_);
	//virtual ~EventListener(){}
	bool Check();
	void OnActivate();
	//!!! Please copy before use !!!
	//auto foo = EventListener::Check_SingleClick_Func;
	static std::function<bool(EventListener*)> Check_SingleClick_Func;
	static std::function<bool(EventListener*)> Check_Hold_Func;
	static std::function<bool(EventListener*)> Check_HoldUntilRelease_Func;
};

