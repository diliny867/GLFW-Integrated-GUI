#include "GUICanvas.h"

#include "EventListener.h"
#include "GUISystem.h"


GUICanvas::GUICanvas():boundingBox(0,0,0,0),guiSystem(nullptr) {
	updateModelMatrix();
}

GUICanvas::GUICanvas(const double x, const double y, const double width, const double height):boundingBox(x,y,width,height) {}

GUICanvas::GUICanvas(const double x,const double y,const double width,const double height,GUISystem*guiSystem_):boundingBox(x,y,width,height),guiSystem(guiSystem_) {
	updateModelMatrix();
}

void GUICanvas::AddListener(EventListener*listener) {
	listener->guiElement = this;
	listeners.push_back(listener);
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
	if(sideSnap == this){ return; }
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

void GUICanvas::updateSizesRecursive(const Side side) {
	//TODO: add check if snaps are identical or cyclic
	 const SideSnap sideSnap = sideSnaps[sideToIndex(side)];
	if(sideSnap.state == SideSnap::NO_SNAP) {
		return;
	}
	if(sideSnap.state == SideSnap::WINDOW) {
		const glm::ivec2 screenSize = guiSystem->GetScreenSize();
		switch(side) {
		case Side::LEFT:
			if(sideSnap.type == SideSnap::SCALE || sideSnap.type == SideSnap::SCALE_STRAIGHT) {
				boundingBox.width = -boundingBox.x+boundingBox.width;
			}
			boundingBox.x = 0;
			break;
		case Side::RIGHT:
			if(sideSnap.type == SideSnap::DRAG || sideSnap.type == SideSnap::DRAG_STRAIGHT) {
				boundingBox.x = screenSize.x-boundingBox.width;
			}else {
				boundingBox.width = screenSize.x-boundingBox.x;
			}
			break;
		case Side::TOP:
			if(sideSnap.type == SideSnap::SCALE || sideSnap.type == SideSnap::SCALE_STRAIGHT) {
				boundingBox.height = -boundingBox.y+boundingBox.height;
			}
			boundingBox.y = 0;
			break;
		case Side::BOTTOM:
			if(sideSnap.type == SideSnap::DRAG || sideSnap.type == SideSnap::DRAG_STRAIGHT) {
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
		GUICanvas* snapCanvas = sideSnap.canvas;
		snapCanvas->updateSizesRecursive(side);
		const double oldX = boundingBox.x;
		const double oldY = boundingBox.y;
		switch(side) {
		case Side::LEFT:
			switch (sideSnap.type) {
			case SideSnap::DRAG:
				boundingBox.x = snapCanvas->boundingBox.GetX2();
				break;
			case SideSnap::DRAG_STRAIGHT:
				boundingBox.x = snapCanvas->boundingBox.x;
				break;
			case SideSnap::SCALE:
				boundingBox.x = snapCanvas->boundingBox.GetX2();
				boundingBox.width = boundingBox.width+oldX-boundingBox.x;
				break;
			case SideSnap::SCALE_STRAIGHT:
				boundingBox.width = boundingBox.GetX2()-snapCanvas->boundingBox.x;
				boundingBox.x = snapCanvas->boundingBox.x;
				break;
			}
			break;
		case Side::RIGHT:
			switch(sideSnap.type) {
			case SideSnap::DRAG:
				boundingBox.x = oldX+snapCanvas->boundingBox.x-boundingBox.GetX2();
				break;
			case SideSnap::DRAG_STRAIGHT:
				boundingBox.x = snapCanvas->boundingBox.GetX2()-boundingBox.width;
				break;
			case SideSnap::SCALE:
				boundingBox.width = snapCanvas->boundingBox.x-boundingBox.x;
				break;
			case SideSnap::SCALE_STRAIGHT:
				boundingBox.width = snapCanvas->boundingBox.GetX2()-boundingBox.x;
				break;
			}
			break;
		case Side::TOP:
			switch(sideSnap.type) {
			case SideSnap::DRAG:
				boundingBox.y = snapCanvas->boundingBox.GetY2();
				break;
			case SideSnap::DRAG_STRAIGHT:
				boundingBox.y = snapCanvas->boundingBox.y;
				break;
			case SideSnap::SCALE:
				boundingBox.y = snapCanvas->boundingBox.GetY2();
				boundingBox.height = boundingBox.height+oldY-boundingBox.y;
				break;
			case SideSnap::SCALE_STRAIGHT:
				boundingBox.height = boundingBox.GetY2()-snapCanvas->boundingBox.y;
				boundingBox.y = snapCanvas->boundingBox.y;
				break;
			}
			break;
		case Side::BOTTOM:
			switch(sideSnap.type) {
			case SideSnap::DRAG:
				boundingBox.y = oldY+snapCanvas->boundingBox.y-boundingBox.GetY2();
				break;
			case SideSnap::DRAG_STRAIGHT:
				boundingBox.y = snapCanvas->boundingBox.GetY2()-boundingBox.height;
				break;
			case SideSnap::SCALE:
				boundingBox.height = snapCanvas->boundingBox.y-boundingBox.y;
				break;
			case SideSnap::SCALE_STRAIGHT:
				boundingBox.height = snapCanvas->boundingBox.GetY2()-boundingBox.y;
				break;
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
		if(sideSnaps[i].type == SideSnap::DRAG || sideSnaps[i].type == SideSnap::DRAG_STRAIGHT) {
			sidesUpdateOrdered.push_front(indexToSide(i));
		}else {
			sidesUpdateOrdered.push_back(indexToSide(i));
		}
	}
	for(const auto& side: sidesUpdateOrdered) {
		updateSizesRecursive(side);
	}
	sidesUpdateOrdered.clear();
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

void GUICanvas::SetTexture(const char* path) {
	texture = new Texture2D(path,TextureType::Diffuse);
}
void GUICanvas::SetTexture(Texture2D* texture_) {
	texture = texture_;
}

void GUICanvas::Undirty() {
	dirty = false;
}

void GUICanvas::updateModelMatrix() {
	model = glm::mat4(1.0f);
	model = glm::translate(model,glm::vec3(boundingBox.x,boundingBox.y+boundingBox.height,0.0f));
	model = glm::scale(model,glm::vec3(boundingBox.width,-boundingBox.height,1.0f));
}

