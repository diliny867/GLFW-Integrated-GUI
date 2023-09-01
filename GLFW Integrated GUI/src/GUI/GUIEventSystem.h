#pragma once

#include <unordered_map>

class EventListener;

class GUIEventSystem {
public:
	enum EventType {
		Always = 0,
		MouseDown,
		LeftMouseDown,
		RightMouseDown,
		MouseUp,
		LeftMouseUp,
		RightMouseUp,
		MouseHold,
		LeftMouseHold,
		RightMouseHold
	};
private:
	inline static std::unordered_map<EventType,std::vector<EventListener*>> listeners;

	inline static bool lastLeftMouse = false;
	inline static bool lastRightMouse = false;
public:
	//static void SubscribeMouseDown(EventListener* el);
	//static void SubscribeLeftMouseDown(EventListener* el);
	//static void SubscribeRightMouseDown(EventListener* el);
	//
	//static void SubscribeMouseUp(EventListener* el);
	//static void SubscribeLeftMouseUp(EventListener* el);
	//static void SubscribeRightMouseUp(EventListener* el);
	//
	//static void SubscribeMouseHold(EventListener* el);
	//static void SubscribeLeftMouseHold(EventListener* el);
	//static void SubscribeRightMouseHold(EventListener* el);
	//
	//static void SubscribeAlways(EventListener* el);

	static void SubscribeEvent(EventListener* el,const EventType event);
	static void DeSubscribeEvent(EventListener* el,const EventType event);
	static void DeSubscribeAllEvents(EventListener* el,const EventType event);

	static void NotifyByEventType(const EventType type);

	static void NotifyMouseDown();
	static void NotifyMouseHold();
	static void NotifyMouseUp();

	static void Init();

	static void TryNotifySubscribers();
};