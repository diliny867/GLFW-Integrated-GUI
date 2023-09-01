#pragma once

#include "GUIGenericInteractable.h"

class GUIButton: public GUIGenericInteractable {
public:
	using GUIGenericInteractable::GUIGenericInteractable;

	using GUIGenericInteractable::SetOnMouseDown;
	using GUIGenericInteractable::SetOnLeftMouseDown;
	using GUIGenericInteractable::SetOnRightMouseDown;
	using GUIGenericInteractable::SetOnMouseDownElse;
	using GUIGenericInteractable::SetOnLeftMouseDownElse;
	using GUIGenericInteractable::SetOnRightMouseDownElse;

	using GUIGenericInteractable::SetOnMouseUp;
	using GUIGenericInteractable::SetOnLeftMouseUp;
	using GUIGenericInteractable::SetOnRightMouseUp;
	using GUIGenericInteractable::SetOnMouseUpElse;
	using GUIGenericInteractable::SetOnLeftMouseUpElse;
	using GUIGenericInteractable::SetOnRightMouseUpElse;

	using GUIGenericInteractable::SetOnMouseHover;
	using GUIGenericInteractable::SetOnMouseHoverElse;
};