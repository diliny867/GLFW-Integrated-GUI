#include "GUICanvas.h"

#include "EventListener.h"
#include "GUISystem.h"


GUICanvas::GUICanvas():boundingBox(0,0,0,0),transform(boundingBox),guiSystem(nullptr) {
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
	return boundingBox.ContainsPoint(x,y); // && transform.ContainsPoint(x,y)
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

//void GUICanvas::updateSizesRecursive(const Side side) {
//	//TODO: add check if snaps are identical or cyclic
//	 const SideSnap sideSnap = sideSnaps[sideToIndex(side)];
//	if(sideSnap.state == SideSnap::NO_SNAP) {
//		return;
//	}
//	if(sideSnap.state == SideSnap::WINDOW) {
//		const glm::ivec2 screenSize = guiSystem->GetScreenSize();
//		switch(side) {
//		case Side::LEFT:
//			if(sideSnap.type == SideSnap::SCALE || sideSnap.type == SideSnap::SCALE_STRAIGHT) {
//				boundingBox.width = -boundingBox.x+boundingBox.width;
//			}
//			boundingBox.x = 0;
//			break;
//		case Side::RIGHT:
//			if(sideSnap.type == SideSnap::DRAG || sideSnap.type == SideSnap::DRAG_STRAIGHT) {
//				boundingBox.x = screenSize.x-boundingBox.width;
//			}else {
//				boundingBox.width = screenSize.x-boundingBox.x;
//			}
//			break;
//		case Side::TOP:
//			if(sideSnap.type == SideSnap::SCALE || sideSnap.type == SideSnap::SCALE_STRAIGHT) {
//				boundingBox.height = -boundingBox.y+boundingBox.height;
//			}
//			boundingBox.y = 0;
//			break;
//		case Side::BOTTOM:
//			if(sideSnap.type == SideSnap::DRAG || sideSnap.type == SideSnap::DRAG_STRAIGHT) {
//				boundingBox.y = screenSize.y-boundingBox.height;
//			}else {
//				boundingBox.height = screenSize.y-boundingBox.y;
//			}
//			break;
//		default:
//			break;
//		}
//	} 
//	if(sideSnap.state == SideSnap::CANVAS){
//		GUICanvas* snapCanvas = sideSnap.canvas;
//		snapCanvas->updateSizesRecursive(side);
//		const double oldX = boundingBox.x;
//		const double oldY = boundingBox.y;
//		switch(side) {
//		case Side::LEFT:
//			switch (sideSnap.type) {
//			case SideSnap::DRAG:
//				boundingBox.x = snapCanvas->boundingBox.GetX2();
//				break;
//			case SideSnap::DRAG_STRAIGHT:
//				boundingBox.x = snapCanvas->boundingBox.x;
//				break;
//			case SideSnap::SCALE:
//				boundingBox.x = snapCanvas->boundingBox.GetX2();
//				boundingBox.width = boundingBox.width+oldX-boundingBox.x;
//				break;
//			case SideSnap::SCALE_STRAIGHT:
//				boundingBox.width = boundingBox.GetX2()-snapCanvas->boundingBox.x;
//				boundingBox.x = snapCanvas->boundingBox.x;
//				break;
//			}
//			break;
//		case Side::RIGHT:
//			switch(sideSnap.type) {
//			case SideSnap::DRAG:
//				boundingBox.x = oldX+snapCanvas->boundingBox.x-boundingBox.GetX2();
//				break;
//			case SideSnap::DRAG_STRAIGHT:
//				boundingBox.x = snapCanvas->boundingBox.GetX2()-boundingBox.width;
//				break;
//			case SideSnap::SCALE:
//				boundingBox.width = snapCanvas->boundingBox.x-boundingBox.x;
//				break;
//			case SideSnap::SCALE_STRAIGHT:
//				boundingBox.width = snapCanvas->boundingBox.GetX2()-boundingBox.x;
//				break;
//			}
//			break;
//		case Side::TOP:
//			switch(sideSnap.type) {
//			case SideSnap::DRAG:
//				boundingBox.y = snapCanvas->boundingBox.GetY2();
//				break;
//			case SideSnap::DRAG_STRAIGHT:
//				boundingBox.y = snapCanvas->boundingBox.y;
//				break;
//			case SideSnap::SCALE:
//				boundingBox.y = snapCanvas->boundingBox.GetY2();
//				boundingBox.height = boundingBox.height+oldY-boundingBox.y;
//				break;
//			case SideSnap::SCALE_STRAIGHT:
//				boundingBox.height = boundingBox.GetY2()-snapCanvas->boundingBox.y;
//				boundingBox.y = snapCanvas->boundingBox.y;
//				break;
//			}
//			break;
//		case Side::BOTTOM:
//			switch(sideSnap.type) {
//			case SideSnap::DRAG:
//				boundingBox.y = oldY+snapCanvas->boundingBox.y-boundingBox.GetY2();
//				break;
//			case SideSnap::DRAG_STRAIGHT:
//				boundingBox.y = snapCanvas->boundingBox.GetY2()-boundingBox.height;
//				break;
//			case SideSnap::SCALE:
//				boundingBox.height = snapCanvas->boundingBox.y-boundingBox.y;
//				break;
//			case SideSnap::SCALE_STRAIGHT:
//				boundingBox.height = snapCanvas->boundingBox.GetY2()-boundingBox.y;
//				break;
//			}
//			break;
//		default:
//			break;
//		}
//	}
//}
//void GUICanvas::updateSizesRecursive(const Side side) {
//	//TODO: add check if snaps are identical or cyclic
//	const SideSnap sideSnap = sideSnaps[sideToIndex(side)];
//	if(sideSnap.state == SideSnap::NO_SNAP) {
//		return;
//	}
//	if(sideSnap.state == SideSnap::WINDOW) {
//		const glm::vec2 screenSize = guiSystem->GetScreenSize();
//		switch(side) {
//		case Side::LEFT:
//			if(sideSnap.type == SideSnap::SCALE || sideSnap.type == SideSnap::SCALE_STRAIGHT) {
//				boundingBox.min.x = 0;
//			}else {
//				boundingBox.min.x = boundingBox.getWidth();
//				boundingBox.max.x = 0;
//			}
//			break;
//		case Side::RIGHT:
//			if(sideSnap.type == SideSnap::SCALE || sideSnap.type == SideSnap::SCALE_STRAIGHT) {
//				boundingBox.max.x = screenSize.x;
//			} else {
//				boundingBox.min.x = screenSize.x-boundingBox.getWidth();
//				boundingBox.max.x = screenSize.x;
//			}
//			break;
//		case Side::TOP:
//			if(sideSnap.type == SideSnap::SCALE || sideSnap.type == SideSnap::SCALE_STRAIGHT) {
//				boundingBox.min.y = 0;
//			} else {
//				boundingBox.max.y = boundingBox.getHeight();
//				boundingBox.min.y = 0;
//			}
//			break;
//		case Side::BOTTOM:
//			if(sideSnap.type == SideSnap::SCALE || sideSnap.type == SideSnap::SCALE_STRAIGHT) {
//				boundingBox.max.y = screenSize.y;
//			} else {
//				boundingBox.min.y = screenSize.y-boundingBox.getHeight();
//				boundingBox.max.y = screenSize.y;
//			}
//			break;
//		default:
//			break;
//		}
//	}
//	if(sideSnap.state == SideSnap::CANVAS){
//		sideSnap.canvas->updateSizesRecursive(side);
//		const BoundingBox& otherBB = sideSnap.canvas->boundingBox;
//		const glm::vec2 oldSize = boundingBox.getSize();
//		switch(side) {
//		case Side::LEFT:
//			switch(sideSnap.type) {
//			case SideSnap::DRAG:
//				boundingBox.min.x = otherBB.max.x;
//				boundingBox.max.x = boundingBox.min.x + oldSize.x;
//				break;
//			case SideSnap::DRAG_STRAIGHT:
//				boundingBox.min.x = otherBB.min.x;
//				boundingBox.max.x = boundingBox.min.x + oldSize.x;
//				break;
//			case SideSnap::SCALE:
//				boundingBox.min.x = otherBB.max.x;
//				break;
//			case SideSnap::SCALE_STRAIGHT:
//				boundingBox.min.x = otherBB.min.x;
//				break;
//			}
//			break;
//		case Side::RIGHT:
//			switch(sideSnap.type) {
//			case SideSnap::DRAG:
//				boundingBox.max.x = otherBB.min.x;
//				boundingBox.min.x = boundingBox.max.x - oldSize.x;
//				break;
//			case SideSnap::DRAG_STRAIGHT:
//				boundingBox.max.x = otherBB.max.x;
//				boundingBox.min.x = boundingBox.max.x - oldSize.x;
//				break;
//			case SideSnap::SCALE:
//				boundingBox.max.x = otherBB.min.x;
//				break;
//			case SideSnap::SCALE_STRAIGHT:
//				boundingBox.max.x = otherBB.max.x;
//				break;
//			}
//			break;
//		case Side::TOP:
//			switch(sideSnap.type) {
//			case SideSnap::DRAG:
//				boundingBox.min.y = otherBB.max.y;
//				boundingBox.max.y = boundingBox.min.y + oldSize.y;
//				break;
//			case SideSnap::DRAG_STRAIGHT:
//				boundingBox.min.y = otherBB.min.y;
//				boundingBox.max.y = boundingBox.min.y + oldSize.y;
//				break;
//			case SideSnap::SCALE:
//				boundingBox.min.y = otherBB.max.y;
//				break;
//			case SideSnap::SCALE_STRAIGHT:
//				boundingBox.min.y = otherBB.min.y;
//				break;
//			}
//			break;
//		case Side::BOTTOM:
//			switch(sideSnap.type) {
//			case SideSnap::DRAG:
//				boundingBox.max.y = otherBB.min.y;
//				boundingBox.min.y = boundingBox.max.y - oldSize.y;
//				break;
//			case SideSnap::DRAG_STRAIGHT:
//				boundingBox.max.y = otherBB.max.y;
//				boundingBox.min.y = boundingBox.max.y - oldSize.y;
//				break;
//			case SideSnap::SCALE:
//				boundingBox.max.y = otherBB.min.y;
//				break;
//			case SideSnap::SCALE_STRAIGHT:
//				boundingBox.max.y = otherBB.max.y;
//				break;
//			}
//			break;
//		default:
//			break;
//		}
//	}
//}
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
	UpdateBoundingBox();
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
	return model*viewTransformMat4;
}

void GUICanvas::Undirty() {
	dirty = false;
}

//void GUICanvas::updateModelMatrix() {
//	model = glm::mat4(1.0f);
//	model = glm::translate(model,glm::vec3(boundingBox.x,boundingBox.y+boundingBox.height,0.0f));
//	model = glm::scale(model,glm::vec3(boundingBox.width,-boundingBox.height,1.0f));
//}
void GUICanvas::updateModelMatrix() {
	//transform.rotation = glm::quat(glm::vec3(0.f,0.f,0.1f));

	model = transform.ToMat4();
}

void GUICanvas::UpdateBoundingBox() {
	boundingBox = transform.CalculateEnclosingBoundingBox();
	MarkDirty();
}
