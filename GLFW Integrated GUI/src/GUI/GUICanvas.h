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
		enum SnapType {
			DRAG,
			SCALE
		};
		SnapState state;
		SnapType type;
		GUICanvas* canvas;
		//GUICanvas* source; //TODO: Add this to update elements snapped to this object (like add something like NotifySnaps() function)
		SideSnap():state(NO_SNAP),type(DRAG),canvas(nullptr) {}
		SideSnap(const SnapState state_):state(state_),type(DRAG),canvas(nullptr) {}
		SideSnap(const SnapState state_,const SnapType type_):state(state_),type(type_),canvas(nullptr){}
		SideSnap(const SnapState state_,const SnapType type_,GUICanvas* parent_):state(state_),type(type_),canvas(parent_){}
	};
	enum Side {
		UNKNOWN = 0,
		LEFT,
		RIGHT,
		TOP,
		BOTTOM
	};
	
protected:
	bool dirty = false;
	static Side indexToSide(const int index);
	static int sideToIndex(const Side side);
	void updateWhenDirty();
	void updateSizesRecursive(const Side side);
	void updateSizesRecursive();
	void updateModelMatrix();
	void notifyAllSnappedToCurrent();
public:
	BoundingBox boundingBox;

	SideSnap sideSnaps[4];
	GUICanvas* snappedToCurrent[4];

	GUISystem* guiSystem;

	glm::mat4 model = glm::mat4(1.0f);
	Texture2D* texture = nullptr;

	GUICanvas();
	GUICanvas(const double x,const double y,const double width,const double height, GUISystem* guiSystem_);

	std::vector<EventListener*> listeners;
	void AddListener(EventListener* listener);
	bool CheckClick(const double x, const double y) const;
	void SetSnap(const Side side, const SideSnap::SnapType sideSnapType, const SideSnap::SnapState sideSnapState);
	void SetSnap(const Side side, const SideSnap::SnapType sideSnapType, const SideSnap::SnapState sideSnapState, GUICanvas* sideSnap);
	void MarkDirty();
	bool IsDirty() const;
	void UpdateView();
	void Undirty();
};