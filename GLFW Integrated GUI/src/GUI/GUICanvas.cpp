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

void GUICanvas::SetSnap(const Side side,const SideSnap::SnapState sideSnapState) {
	const SideSnap snap = SideSnap(sideSnapState);
	switch(side) {
	case Side::LEFT:
		sideSnaps[0] = snap;
		break;
	case Side::RIGHT:
		sideSnaps[1] = snap;
		break;
	case Side::TOP:
		sideSnaps[2] = snap;
		break;
	case Side::BOTTOM:
		sideSnaps[3] = snap;
		break;
	default:
		break;
	}
	MarkDirty();
}
void GUICanvas::SetSnap(const Side side,const SideSnap::SnapState sideSnapState, GUICanvas* sideSnap) {
	const SideSnap snap = SideSnap(sideSnapState,sideSnap);
	switch(side) {
	case Side::LEFT:
		sideSnaps[0] = snap;
		break;
	case Side::RIGHT:
		sideSnaps[1] = snap;
		break;
	case Side::TOP:
		sideSnaps[2] = snap;
		break;
	case Side::BOTTOM:
		sideSnaps[3] = snap;
		break;
	default:
		break;
	}
	MarkDirty();
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
	switch(side) {
	case Side::LEFT:
		sideSnap = sideSnaps[0];
		break;
	case Side::RIGHT:
		sideSnap = sideSnaps[1];
		break;
	case Side::TOP:
		sideSnap = sideSnaps[2];
		break;
	case Side::BOTTOM:
		sideSnap = sideSnaps[3];
		break;
	default:
		break;
	}
	if(sideSnap.state == SideSnap::NO_SNAP) {
		return;
	}
	if(sideSnap.state == SideSnap::WINDOW) {
		switch(side) {
		case Side::LEFT:
			boundingBox.x = 0;
			break;
		case Side::RIGHT:
			boundingBox.width = guiSystem->screenSize.x-boundingBox.x;
			break;
		case Side::TOP:
			boundingBox.y = 0;
			break;
		case Side::BOTTOM:
			boundingBox.height = guiSystem->screenSize.y-boundingBox.y;
			break;
		default:
			break;
		}
	} 
	if(sideSnap.state == SideSnap::CANVAS){
		sideSnap.canvas->updateSizesRecursive(side);
		switch(side) {
		case Side::LEFT:
			boundingBox.x = sideSnap.canvas->boundingBox.GetX2();
			break;
		case Side::RIGHT:
			boundingBox.width = sideSnap.canvas->boundingBox.x-boundingBox.x;
			break;
		case Side::TOP:
			boundingBox.y = sideSnap.canvas->boundingBox.GetY2();
			break;
		case Side::BOTTOM:
			boundingBox.height = sideSnap.canvas->boundingBox.y-boundingBox.y;
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
		Undirty();
	}

	//if(dirty) {
	//	UpdateWhenDirty();
	//	Undirty();
	//	return;
	//}
	//for(int i=0;i<4;i++) {
	//	if(sideSnaps[i].sideSnapState!=SideSnap::NO_SNAP) {
	//		MarkDirty();
	//		UpdateWhenDirty();
	//		break;
	//	}
	//}
	//Undirty();
}
void GUICanvas::Undirty() {
	dirty = false;
}

void GUICanvas::updateModelMatrix() {
	model = glm::mat4(1.0f);
	model = glm::translate(model,glm::vec3(boundingBox.x,boundingBox.y+boundingBox.height,0.0f));
	model = glm::scale(model,glm::vec3(boundingBox.width,-boundingBox.height,1.0f));
}

