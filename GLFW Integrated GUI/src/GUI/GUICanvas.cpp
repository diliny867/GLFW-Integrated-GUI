#include "GUICanvas.h"

#include "EventListener.h"
#include "GUISystem.h"


GUICanvas::GUICanvas():boundingBox(0,0,0,0),guiSystem(nullptr) {
	updateModelMatrix();
}
GUICanvas::GUICanvas(const double x,const double y,const double width,const double height,GUISystem*guiSystem_):boundingBox(x,y,width,height),guiSystem(guiSystem_) {
	updateModelMatrix();
}

void GUICanvas::AddListener(EventListener*listener) {
	listeners.push_back(listener);
	listeners.back()->guiElement = this;
}

bool GUICanvas::CheckClick(const double x,const double y) const {
	return boundingBox.ContainsPoint(x,y);
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
	const SideSnap newSnap = SideSnap(sideSnapState,sideSnapType,sideSnap);
	const int snapIndex = sideToIndex(side);
	if(sideSnaps[snapIndex] == newSnap) {
		return;
	}
	sideSnaps[snapIndex] = newSnap;
	sideSnap->snappedToThis.push_back(this);
	MarkDirty();
}
void GUICanvas::notifyAllSnappedToThis() {
	for(const auto& snap: snappedToThis) {
		snap->MarkDirty();
		snap->UpdateView();
	}
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
	guiSystem->MarkDirty();
}
bool GUICanvas::IsDirty() const {
	return dirty;
}

void GUICanvas::updateSizesRecursive(const Side side) { //sorry for 3 identical switches
	//TODO: add check if snaps are identical or cyclic
	SideSnap sideSnap;
	sideSnap = sideSnaps[sideToIndex(side)];
	if(sideSnap.state == SideSnap::NO_SNAP) {
		return;
	}
	if(sideSnap.state == SideSnap::WINDOW) {
		const glm::ivec2 screenSize = guiSystem->GetScreenSize();
		switch(side) {
		case Side::LEFT:
			if(sideSnap.type == SideSnap::SCALE) {
				boundingBox.width = -boundingBox.x+boundingBox.width;
			}
			boundingBox.x = 0;
			break;
		case Side::RIGHT:
			if(sideSnap.type == SideSnap::DRAG) {
				boundingBox.x = screenSize.x-boundingBox.width;
			}else {
				boundingBox.width = screenSize.x-boundingBox.x;
			}
			break;
		case Side::TOP:
			if(sideSnap.type == SideSnap::SCALE) {
				boundingBox.height = -boundingBox.y+boundingBox.height;
			}
			boundingBox.y = 0;
			break;
		case Side::BOTTOM:
			if(sideSnap.type == SideSnap::DRAG) {
				boundingBox.y = screenSize.y-boundingBox.height;
			}else {
				boundingBox.height = screenSize.y-boundingBox.y;
			}
			break;
		default:
			break;
		}
	} 
	if(sideSnap.state == SideSnap::CANVAS){
		sideSnap.canvas->updateSizesRecursive(side);
		const double oldX = boundingBox.x;
		const double oldY = boundingBox.y;
		switch(side) {
		case Side::LEFT:
			boundingBox.x = sideSnap.canvas->boundingBox.GetX2();
			if(sideSnap.type == SideSnap::SCALE) {
				boundingBox.width = boundingBox.width+oldX-boundingBox.x;
			}
			break;
		case Side::RIGHT:
			if(sideSnap.type == SideSnap::DRAG) {
				boundingBox.x = oldX+sideSnap.canvas->boundingBox.x-boundingBox.GetX2();
			}else {
				boundingBox.width = sideSnap.canvas->boundingBox.x-boundingBox.x;
			}
			break;
		case Side::TOP:
			boundingBox.y = sideSnap.canvas->boundingBox.GetY2();
			if(sideSnap.type == SideSnap::SCALE) {
				boundingBox.height = boundingBox.height+oldY-boundingBox.y;
			}
			break;
		case Side::BOTTOM:
			if(sideSnap.type == SideSnap::DRAG) {
				boundingBox.y = oldY+sideSnap.canvas->boundingBox.y-boundingBox.GetY2();
			} else{
				boundingBox.height = sideSnap.canvas->boundingBox.y-boundingBox.y;
			}
			break;
		default:
			break;
		}
	}
}
void GUICanvas::updateSizesRecursive() {
	if(!dirty) {
		return;
	}
	for(int i=0;i<4;i++) {
		updateSizesRecursive(indexToSide(i));
	}
}


void GUICanvas::updateWhenDirty() {
	updateSizesRecursive();
	updateModelMatrix();
}

void GUICanvas::UpdateView() {
	if(dirty) {
		updateWhenDirty();
		notifyAllSnappedToThis();
		Undirty();
	}
}
void GUICanvas::Undirty() {
	dirty = false;
}

void GUICanvas::updateModelMatrix() {
	model = glm::mat4(1.0f);
	model = glm::translate(model,glm::vec3(boundingBox.x,boundingBox.y+boundingBox.height,0.0f));
	model = glm::scale(model,glm::vec3(boundingBox.width,-boundingBox.height,1.0f));
}

