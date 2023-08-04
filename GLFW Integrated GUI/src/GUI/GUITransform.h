#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "BoundingBox.h"
#include "GUICommon.h"

class GUITransform {
public:
	glm::vec2 position;
	glm::vec2 size;
	glm::quat rotation;
	GUITransform():position(0.0f),size(0.0f),rotation(glm::vec3(0.0f)){}
	GUITransform(const glm::vec2 position_,const glm::vec2 size_,const glm::quat rotation_):position(position_),size(size_),rotation(rotation_){}
	explicit GUITransform(const BoundingBox& bb):position(bb.min),size(bb.GetSize()),rotation(glm::vec3(0.0f)){}

	glm::mat4 ToMat4() const {
		return glm::scale(glm::translate(glm::mat4(1.0f),glm::vec3(position,0.0f)),glm::vec3(size,1.0f)) * glm::mat4_cast(rotation);
	}
	glm::mat4 ToMat4(const float layer) const { //because translation needs to be applied before rotation (i cant do it in shader)
		return glm::scale(glm::translate(glm::mat4(1.0f),glm::vec3(position,layer)),glm::vec3(size,1.0f)) * glm::mat4_cast(rotation);
	}
	bool ContainsPoint(const float pointX, const float pointY) const {
		const glm::vec2 translatedPoint = glm::vec2(pointX,pointY) - position;
		glm::vec2 rotatedPoint = glm::vec2(rotation * glm::vec3(translatedPoint,0.0f));
		return rotatedPoint.x >= 0 && rotatedPoint.x <= size.x && rotatedPoint.y >= 0 && rotatedPoint.y <= size.y;
	}
	bool ContainsPoint(const glm::vec2& point) const {
		const glm::vec2 translatedPoint = point - position;
		glm::vec2 rotatedPoint = glm::vec2(rotation * glm::vec3(translatedPoint,0.0f));
		return rotatedPoint.x >= 0 && rotatedPoint.x <= size.x && rotatedPoint.y >= 0 && rotatedPoint.y <= size.y;
	}

	float GetX2() const {
		return position.x+size.x;
	}
	float GetY2() const {
		return position.y+size.y;
	}
	glm::vec2 GetMax() const {
		return position+size;
	}

	BoundingBox CalculateEnclosingBoundingBox() const {
		const glm::mat4 model = ToMat4();

		const glm::vec2 c1 = glm::vec2(model*glm::vec4(0.0f,0.0f,0.0f,0.0f));
		const glm::vec2 c2 = glm::vec2(model*glm::vec4(1.0f,0.0f,0.0f,0.0f));
		const glm::vec2 c3 = glm::vec2(model*glm::vec4(0.0f,1.0f,0.0f,0.0f));
		const glm::vec2 c4 = glm::vec2(model*glm::vec4(1.0f,1.0f,0.0f,0.0f));
		
		const glm::vec2 minMaxX = minMaxFrom4(c1.x,c2.x,c3.x,c4.x) + position.x;
		const glm::vec2 minMaxY = minMaxFrom4(c1.y,c2.y,c3.y,c4.y) + position.y;
		
		return BoundingBox(minMaxX.x,minMaxY.x,minMaxX.y,minMaxY.y);
	}

	friend bool operator==(const GUITransform& lhs, const GUITransform& rhs) {
		return lhs.position==rhs.position&&lhs.size==rhs.size&&lhs.rotation==rhs.rotation;
	}
	friend bool operator!=(const GUITransform& lhs,const GUITransform& rhs) {
		return !(lhs==rhs);
	}
};