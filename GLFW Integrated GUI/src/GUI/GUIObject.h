#pragma once

#include <vector>
#include <functional>
#include <unordered_set>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "BoundingBox.h"
#include "GUICommon.h"
#include "GUITransform.h"
#include "GUIObjectViewData.h"

class GUISystem;
class EventListener;
class Texture2D;

class GUIObject {
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
		GUIObject* canvas;
		SideSnap():state(NO_SNAP),type(DRAG),canvas(nullptr) {}
		SideSnap(const SnapState state_):state(state_),type(DRAG),canvas(nullptr) {}
		SideSnap(const SnapState state_,const SnapType type_):state(state_),type(type_),canvas(nullptr){}
		SideSnap(const SnapState state_,const SnapType type_,GUIObject* parent_):state(state_),type(type_),canvas(parent_){}
		friend bool operator==(const SideSnap& lhs,const SideSnap& rhs){ return lhs.state==rhs.state && lhs.type==rhs.type && lhs.canvas==rhs.canvas; }
	};
	enum Side {
		UNKNOWN = 0,
		LEFT,
		RIGHT,
		TOP,
		BOTTOM
	};
	
	std::string label;
protected:
	bool dirty = false;
	static Side indexToSide(const int index);
	static int sideToIndex(const Side side);
	void updateWhenDirty();
	void updateSizesRecursive(const Side side);
	void updateSizesRecursive();
	void updateModelMatrix();
	void notifyAllSnappedToThis();
	void updateBoundingBox();
	BoundingBox boundingBox;
	GUILayer layer=0;
public:

	glm::mat4 model = glm::mat4(1.0f);

	glm::mat4 localViewTransformMat4 = glm::mat4(1.0f);

	GUITransform transform;

	SideSnap sideSnaps[4];
	std::unordered_set<GUIObject*> snappedToThis;

	std::vector<EventListener*> listeners;

	GUISystem* guiSystem;

	Texture2D* texture = nullptr;

	GUIObjectViewData viewData;

	GUIObject();
	GUIObject(GUISystem* guiSystem_);
	GUIObject(const float x,const float y,const float width,const float height);
	GUIObject(const float x,const float y,const float width,const float height, GUISystem* guiSystem_);
	GUIObject(const std::string& label_);
	GUIObject(const std::string& label_,GUISystem* guiSystem_);
	GUIObject(const std::string& label_,const float x,const float y,const float width,const float height);
	GUIObject(const std::string& label_,const float x,const float y,const float width,const float height, GUISystem* guiSystem_);

	void AddListener(EventListener* listener);
	bool CheckClick(const float x, const float y) const;
	void SetSnap(const Side side, const SideSnap::SnapType sideSnapType, const SideSnap::SnapState sideSnapState);
	void SetSnap(const Side side, const SideSnap::SnapType sideSnapType, const SideSnap::SnapState sideSnapState, GUIObject* sideSnap);
	void MarkDirty();
	bool IsDirty() const;
	void UpdateView();
	BoundingBox GetBoundingBox() const;
	void SetTexture(const char* path);
	void SetTexture(Texture2D* texture_);
	glm::mat4 GetDrawModelMatrix() const;
	void SetLayer(const GUILayer layer_);
	GUILayer GetLayer() const;
	void Undirty();
};