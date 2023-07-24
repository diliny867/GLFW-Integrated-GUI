#pragma once

#include <vector>
#include <functional>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../../include/GL/Texture2D.h"
#include "../../include/GL/Shader.h"

#include "BoundingBox.h"
#include "GUICommon.h"
#include "GUITransform.h"

#include <deque>

class EventListener;
class GUISystem;

class GUICanvas {
public:
	struct SideSnap { //Needs refactoring
		enum SnapState {
			NO_SNAP = 0,
			WINDOW,
			CANVAS
		};
		enum SnapType {
			DRAG, //moves to opposite side of snap
			DRAG_STRAIGHT, //moves to the side of snap
			SCALE, //scales to opposite side of snap
			SCALE_STRAIGHT //scales to the side of snap
		};
		SnapState state;
		SnapType type;
		GUICanvas* canvas;
		//GUICanvas* source; //TODO: Add this to update elements snapped to this object (like add something like NotifySnaps() function)
		SideSnap():state(NO_SNAP),type(DRAG),canvas(nullptr) {}
		SideSnap(const SnapState state_):state(state_),type(DRAG),canvas(nullptr) {}
		SideSnap(const SnapState state_,const SnapType type_):state(state_),type(type_),canvas(nullptr){}
		SideSnap(const SnapState state_,const SnapType type_,GUICanvas* parent_):state(state_),type(type_),canvas(parent_){}
		friend bool operator==(const SideSnap& lhs,const SideSnap& rhs){ return lhs.state==rhs.state && lhs.type==rhs.type && lhs.canvas==rhs.canvas; }
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
	void notifyAllSnappedToThis();
	BoundingBox boundingBox;
public:

	GUILayer layer=0;

	glm::mat4 model = glm::mat4(1.0f);

	glm::mat4 viewTransform = glm::mat4(1.0f);

	GUITransform transform;

	SideSnap sideSnaps[4];
	std::vector<GUICanvas*> snappedToThis;

	std::vector<EventListener*> listeners;

	GUISystem* guiSystem;

	Texture2D* texture = nullptr;

	GUICanvas();
	//GUICanvas(const double x,const double y,const double width,const double height);
	//GUICanvas(const double x,const double y,const double width,const double height, GUISystem* guiSystem_);
	GUICanvas(const float x,const float y,const float width,const float height);
	GUICanvas(const float x,const float y,const float width,const float height, GUISystem* guiSystem_);

	void AddListener(EventListener* listener);
	bool CheckClick(const float x, const float y) const;
	void SetSnap(const Side side, const SideSnap::SnapType sideSnapType, const SideSnap::SnapState sideSnapState);
	void SetSnap(const Side side, const SideSnap::SnapType sideSnapType, const SideSnap::SnapState sideSnapState, GUICanvas* sideSnap);
	void MarkDirty();
	bool IsDirty() const;
	void UpdateView();
	void UpdateBoundingBox();
	void SetTexture(const char* path);
	void SetTexture(Texture2D* texture_);
	glm::mat4 GetDrawModelMatrix() const;
	void Undirty();
};