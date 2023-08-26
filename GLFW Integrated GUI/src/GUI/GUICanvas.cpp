#include "GUICanvas.h"

#include "EventListener.h"
#include "GUISystem.h"


GUICanvas::GUICanvas():boundingBox(0,0,0,0),transform(boundingBox),guiSystem(nullptr) {
	updateModelMatrix();
}
GUICanvas::GUICanvas(GUISystem* guiSystem_):boundingBox(0,0,0,0),transform(boundingBox),guiSystem(guiSystem_) {
	updateModelMatrix();
}
//GUICanvas::GUICanvas(const double x, const double y, const double width, const double height):boundingBox(x,y,width,height),guiSystem(nullptr) {
//	updateModelMatrix();
//}
//GUICanvas::GUICanvas(const double x,const double y,const double width,const double height,GUISystem*guiSystem_):boundingBox(x,y,width,height),guiSystem(guiSystem_) {
//	updateModelMatrix();
//}
GUICanvas::GUICanvas(const float x,const float y,const float width,const float height):boundingBox(x,y,x+width,y+height),transform(boundingBox),guiSystem(nullptr) {
	updateModelMatrix();
}
GUICanvas::GUICanvas(const float x,const float y,const float width,const float height,GUISystem* guiSystem_):boundingBox(x,y,x+width,y+height),transform(boundingBox),guiSystem(guiSystem_) {
	updateModelMatrix();
}

void GUICanvas::AddListener(EventListener*listener) {
	listener->guiElement = this;
	listeners.push_back(listener);
}

bool GUICanvas::CheckClick(const float x,const float y) const {
#ifdef BB_CHECK_TRANSFORM_FOR_CLICK
	return boundingBox.ContainsPoint(x,y) && transform.ContainsPoint(x,y);
#else
	return boundingBox.ContainsPoint(x,y);
#endif
}

void GUICanvas::SetSnap(const Side side,const SideSnap::SnapType sideSnapType,const SideSnap::SnapState sideSnapState) {
	const SideSnap newSnap = SideSnap(sideSnapState,sideSnapType);
	const int snapIndex = sideToIndex(side);
	if(sideSnaps[snapIndex] == newSnap) {
		return;
	}
	sideSnaps[snapIndex] = newSnap;
	MarkDirty();
}
void GUICanvas::SetSnap(const Side side,const SideSnap::SnapType sideSnapType,const SideSnap::SnapState sideSnapState,GUICanvas* sideSnap) {
	if(sideSnap == this || sideSnapState != SideSnap::CANVAS){ return; }
	const SideSnap newSnap = SideSnap(sideSnapState,sideSnapType,sideSnap);
	const int snapIndex = sideToIndex(side);
	if(sideSnaps[snapIndex] == newSnap) {
		return;
	}
	sideSnaps[snapIndex] = newSnap;
	sideSnap->snappedToThis.emplace(this);
	MarkDirty();
}
void GUICanvas::notifyAllSnappedToThis() {
	for(const auto snap: snappedToThis) {
		snap->MarkDirty();
	}
	//for(const auto& snap: snappedToThis) {
	//	snap->UpdateView();
	//}
}

GUICanvas::Side GUICanvas::indexToSide(const int index) {
	switch(index) {
	case 0:
		return LEFT;
	case 1:
		return RIGHT;
	case 2:
		return TOP;
	case 3:
		return BOTTOM;
	default:
		return UNKNOWN;
	}
}
int GUICanvas::sideToIndex(const Side side) {
	switch(side) {
	case LEFT:
		return 0;
	case RIGHT:
		return 1;
	case TOP:
		return 2;
	case BOTTOM:
		return 3;
	default:
		return -1;
	}
}

void GUICanvas::MarkDirty() {
	dirty = true;
	if(guiSystem!=nullptr){
		guiSystem->MarkDirty();
		guiSystem->updateQueue.insert(this);
	}
}
bool GUICanvas::IsDirty() const {
	return dirty;
}

void GUICanvas::updateSizesRecursive(const Side side) {
	//TODO: add check if snaps are identical or cyclic
	 const SideSnap sideSnap = sideSnaps[sideToIndex(side)];
	if(sideSnap.state == SideSnap::NO_SNAP || side == UNKNOWN) {
		return;
	}
	GUITransform tr = transform;
	//std::cout<<"Update\n";
	if(sideSnap.state == SideSnap::WINDOW) {
		const glm::vec2 screenSize = guiSystem->GetScreenSize();
		switch(side) {
		case Side::LEFT:
			switch(sideSnap.type) {
			case SideSnap::DRAG:
			case SideSnap::DRAG_STRAIGHT:
				transform.position.x = transform.GetHalfWidth();
				break;
			case SideSnap::SCALE:
			case SideSnap::SCALE_STRAIGHT:
				transform.position.x = transform.GetX2()*0.5f;
				transform.size.x = transform.position.x*2.0f;
				break;
			}
			break;
		case Side::RIGHT:
			switch(sideSnap.type) {
			case SideSnap::DRAG:
			case SideSnap::DRAG_STRAIGHT:
				transform.position.x = screenSize.x-transform.GetHalfWidth();
				break;
			case SideSnap::SCALE:
			case SideSnap::SCALE_STRAIGHT:
				transform.position.x = transform.GetX1()+(screenSize.x-transform.GetX1())*0.5f;
				transform.size.x = (screenSize.x-transform.position.x)*2.0f;
				break;
			}
			break;
		case Side::TOP:
			switch(sideSnap.type) {
			case SideSnap::DRAG:
			case SideSnap::DRAG_STRAIGHT:
				transform.position.y = transform.GetHalfHeight();
				break;
			case SideSnap::SCALE:
			case SideSnap::SCALE_STRAIGHT:
				transform.position.y = transform.GetY2()*0.5f;
				transform.size.y = transform.position.y*2.0f;
				break;
			}
			break;
		case Side::BOTTOM:
			switch(sideSnap.type) {
			case SideSnap::DRAG:
			case SideSnap::DRAG_STRAIGHT:
				transform.position.y = screenSize.y-transform.GetHalfHeight();
				break;
			case SideSnap::SCALE:
			case SideSnap::SCALE_STRAIGHT:
				transform.position.y = transform.GetY1()+(screenSize.y-transform.GetY1())*0.5f;
				transform.size.y = (screenSize.y-transform.position.y)*2.0f;
				break;
			}
			break;
		default:
			break;
		}
	} 
	if(sideSnap.state == SideSnap::CANVAS){
		const glm::vec2 lastMax = transform.GetMax();
		const glm::vec2 lastMin = transform.GetMin();
		const glm::vec2 otherMax = sideSnap.canvas->transform.GetMax();
		const glm::vec2 otherMin = sideSnap.canvas->transform.GetMin();
		switch(side) {
		case Side::LEFT:
			switch (sideSnap.type) {
			case SideSnap::DRAG:
				transform.position.x = otherMax.x+transform.GetHalfWidth();
				break;
			case SideSnap::DRAG_STRAIGHT:
				transform.position.x = otherMin.x+transform.GetHalfWidth();
				break;
			case SideSnap::SCALE:
				transform.position.x = otherMax.x+(lastMax.x-otherMax.x)*0.5f;
				transform.size.x = (lastMax.x-transform.position.x)*2.0f;
				break;
			case SideSnap::SCALE_STRAIGHT:
				transform.position.x = otherMin.x+(lastMax.x-otherMin.x)*0.5f;
				transform.size.x = (lastMax.x-transform.position.x)*2.0f;
				break;
			}
			break;
		case Side::RIGHT:
			switch(sideSnap.type) {
			case SideSnap::DRAG:
				transform.position.x = otherMin.x-transform.GetHalfWidth();
				break;
			case SideSnap::DRAG_STRAIGHT:
				transform.position.x = otherMax.x-transform.GetHalfWidth();
				break;
			case SideSnap::SCALE:
				transform.position.x = lastMin.x+(otherMin.x-lastMin.x)*0.5f;
				transform.size.x = (transform.position.x-lastMin.x)*2.0f;
				break;
			case SideSnap::SCALE_STRAIGHT:
				transform.position.x = lastMin.x+(otherMax.x-lastMin.x)*0.5f;
				transform.size.x = (transform.position.x-lastMin.x)*2.0f;
				break;
			}
			break;
		case Side::TOP:
			switch(sideSnap.type) {
			case SideSnap::DRAG:
				transform.position.y = otherMax.y+transform.GetHalfHeight();
				break;
			case SideSnap::DRAG_STRAIGHT:
				transform.position.y = otherMin.y+transform.GetHalfHeight();
				break;
			case SideSnap::SCALE:
				transform.position.y = otherMax.y+(lastMax.y-otherMax.y)*0.5f;
				transform.size.y = (lastMax.y-transform.position.y)*2.0f;
				break;
			case SideSnap::SCALE_STRAIGHT:
				transform.position.y = otherMin.y+(lastMax.y-otherMin.y)*0.5f;
				transform.size.y = (lastMax.y-transform.position.y)*2.0f;
				break;
			}
			break;
		case Side::BOTTOM:
			switch(sideSnap.type) {
			case SideSnap::DRAG:
				transform.position.y = otherMin.y-transform.GetHalfHeight();
				break;
			case SideSnap::DRAG_STRAIGHT:
				transform.position.y = otherMax.y-transform.GetHalfHeight();
				break;
			case SideSnap::SCALE:
				transform.position.y = lastMin.y+(otherMin.y-lastMin.y)*0.5f;
				transform.size.y = (transform.position.y-lastMin.y)*2.0f;
				break;
			case SideSnap::SCALE_STRAIGHT:
				transform.position.y = lastMin.y+(otherMax.y-lastMin.y)*0.5f;
				transform.size.y = (transform.position.y-lastMin.y)*2.0f;
				break;
			}
			break;
		default:
			break;
		}
		if(tr!=transform){ //or else (bad things)
			//sideSnap.canvas->MarkDirty();
		}
	}
}
/*
void GUICanvas::updateSizesRecursive(const Side side) {
	//TODO: add check if snaps are identical or cyclic
	 const SideSnap sideSnap = sideSnaps[sideToIndex(side)];
	if(sideSnap.state == SideSnap::NO_SNAP) {
		return;
	}
	if(sideSnap.state == SideSnap::WINDOW) {
		const glm::vec2 screenSize = guiSystem->GetScreenSize();
		switch(side) {
		case Side::LEFT:
			if(sideSnap.type == SideSnap::SCALE || sideSnap.type == SideSnap::SCALE_STRAIGHT) {
				transform.size.x = -transform.position.x+transform.size.x;
			}
			transform.position.x = 0;
			break;
		case Side::RIGHT:
			if(sideSnap.type == SideSnap::DRAG || sideSnap.type == SideSnap::DRAG_STRAIGHT) {
				transform.position.x = screenSize.x-transform.size.x;
			}else {
				transform.size.x = screenSize.x-transform.position.x;
			}
			break;
		case Side::TOP:
			if(sideSnap.type == SideSnap::SCALE || sideSnap.type == SideSnap::SCALE_STRAIGHT) {
				transform.size.y = -transform.position.y+transform.size.y;
			}
			transform.position.y = 0;
			break;
		case Side::BOTTOM:
			if(sideSnap.type == SideSnap::DRAG || sideSnap.type == SideSnap::DRAG_STRAIGHT) {
				transform.position.y = screenSize.y-transform.size.y;
			}else {
				transform.size.y = screenSize.y-transform.position.y;
			}
			break;
		default:
			break;
		}
	}
	if(sideSnap.state == SideSnap::CANVAS){
		sideSnap.canvas->updateSizesRecursive(side);
		GUITransform& otherTransform = sideSnap.canvas->transform;
		const glm::vec2 oldPosition = transform.position;
		switch(side) {
		case Side::LEFT:
			switch (sideSnap.type) {
			case SideSnap::DRAG:
				transform.position.x = otherTransform.GetX2();
				break;
			case SideSnap::DRAG_STRAIGHT:
				transform.position.x = otherTransform.position.x;
				break;
			case SideSnap::SCALE:
				transform.position.x = otherTransform.GetX2();
				transform.size.x = transform.size.x+oldPosition.x-transform.position.x;
				break;
			case SideSnap::SCALE_STRAIGHT:
				transform.size.x = transform.GetX2()-otherTransform.position.x;
				transform.position.x = otherTransform.position.x;
				break;
			}
			break;
		case Side::RIGHT:
			switch(sideSnap.type) {
			case SideSnap::DRAG:
				transform.position.x = transform.position.x+otherTransform.position.x-transform.GetX2();
				break;
			case SideSnap::DRAG_STRAIGHT:
				transform.position.x = otherTransform.GetX2()-transform.size.x;
				break;
			case SideSnap::SCALE:
				transform.size.x = otherTransform.position.x-transform.position.x;
				break;
			case SideSnap::SCALE_STRAIGHT:
				transform.size.x = otherTransform.GetX2()-transform.position.x;
				break;
			}
			break;
		case Side::TOP:
			switch(sideSnap.type) {
			case SideSnap::DRAG:
				transform.position.y = otherTransform.GetY2();
				break;
			case SideSnap::DRAG_STRAIGHT:
				transform.position.y = otherTransform.position.y;
				break;
			case SideSnap::SCALE:
				transform.position.y = otherTransform.GetY2();
				transform.size.y = transform.size.y+oldPosition.y-transform.position.y;
				break;
			case SideSnap::SCALE_STRAIGHT:
				transform.size.y = transform.GetY2()-otherTransform.position.y;
				transform.position.y = otherTransform.position.y;
				break;
			}
			break;
		case Side::BOTTOM:
			switch(sideSnap.type) {
			case SideSnap::DRAG:
				transform.position.y = transform.position.y+otherTransform.position.y-transform.GetY2();
				break;
			case SideSnap::DRAG_STRAIGHT:
				transform.position.y = otherTransform.GetY2()-transform.size.y;
				break;
			case SideSnap::SCALE:
				transform.size.y = otherTransform.position.y-transform.position.y;
				break;
			case SideSnap::SCALE_STRAIGHT:
				transform.size.y = otherTransform.GetY2()-transform.position.y;
				break;
			}
			break;
		default:
			break;
		}
	}
}
 */

void GUICanvas::updateSizesRecursive() {
	if(!dirty) {
		return;
	}

	std::deque<Side> sidesUpdateOrdered;
	for(int i=0;i<4;i++) {
		if(sideSnaps[i].type == SideSnap::DRAG || sideSnaps[i].type == SideSnap::DRAG_STRAIGHT) {
			sidesUpdateOrdered.push_front(indexToSide(i));
		}else {
			sidesUpdateOrdered.push_back(indexToSide(i));
		}
	}
	for(const auto& side: sidesUpdateOrdered) {
		updateSizesRecursive(side);
	}
}


void GUICanvas::updateWhenDirty() {
	updateSizesRecursive();
	updateBoundingBox();
	updateModelMatrix();
}

void GUICanvas::UpdateView() {
	if(dirty) {
		updateWhenDirty();
		notifyAllSnappedToThis();
		Undirty();
	}
}

void GUICanvas::SetTexture(const char* path) {
	texture = new Texture2D(path,TextureType::Diffuse);
}
void GUICanvas::SetTexture(Texture2D* texture_) {
	texture = texture_;
}

glm::mat4 GUICanvas::GetDrawModelMatrix() const {
	return model*localViewTransformMat4;
}

void GUICanvas::SetLayer(const GUILayer layer_) {
	if(layer_>LAYER_MAX) {
		return;
	}
	if(guiSystem==nullptr) {
		layer = layer_;
		return;
	}
	guiSystem->RemoveCanvasElement(this);
	layer = layer_;
	guiSystem->AddCanvasElement(this);

}
GUILayer GUICanvas::GetLayer() const {
	return layer;
}

void GUICanvas::Undirty() {
	dirty = false;
}

void GUICanvas::updateModelMatrix() {
	//transform.rotation = glm::quat(glm::vec3(-0.3f));

	model = transform.ToMat4(layer);
}

void GUICanvas::updateBoundingBox() {
	boundingBox = transform.CalculateEnclosingBoundingBox();
	MarkDirty();
}

BoundingBox GUICanvas::GetBoundingBox() const {
	return boundingBox;
}