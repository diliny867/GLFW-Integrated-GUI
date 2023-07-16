#pragma once

#include <vector>
#include <functional>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../../include/GL/Texture2D.h"

#include "BoundingBox.h"
#include "GUICommon.h"

class EventListener;
class GUISystem;

class GUICanvas {
public:
	struct SideSnap {
		enum SnapState {
			NO_SNAP = 0,
			WINDOW,
			CANVAS
		};
		SnapState state;
		GUICanvas* canvas;
		//GUICanvas* source; //TODO: Add this to update elements snapped to this object (like add something like NotifySnaps() function)
		SideSnap():state(NO_SNAP),canvas(nullptr){}
		SideSnap(const SnapState state_):state(state_),canvas(nullptr){}
		SideSnap(const SnapState state_, GUICanvas* parent_):state(state_),canvas(parent_){}
	};
	enum Side {
		UNKNOWN = 0,
		LEFT,
		RIGHT,
		TOP,
		BOTTOM
	};
	enum PositionState { //css :crying: emoji, :skull: emoji
		Abosolute,
		Fixed,
		Relative
	};
protected:
	bool dirty = false;
	static Side indexToSide(const int index);
	static int sideToIndex(const Side side);
	void updateWhenDirty();
	void updateSizesRecursive(const Side side);
	void updateSizesRecursive();
	void updateModelMatrix();
public:
	BoundingBox boundingBox;

	PositionState posState;
	SideSnap sideSnaps[4] ;

	GUISystem* guiSystem;

	glm::mat4 model = glm::mat4(1.0f);
	Texture2D* texture = nullptr;

	GUICanvas();
	GUICanvas(const double x,const double y,const double width,const double height, GUISystem* guiSystem_);

	std::vector<EventListener*> listeners;
	void AddListener(EventListener* listener);
	bool CheckClick(const double x, const double y) const;
	void SetSnap(const Side side, const SideSnap::SnapState sideSnapState);
	void SetSnap(const Side side, const SideSnap::SnapState sideSnapState, GUICanvas* sideSnap);
	void MarkDirty();
	bool IsDirty() const;
	void UpdateView();
	void Undirty();
};