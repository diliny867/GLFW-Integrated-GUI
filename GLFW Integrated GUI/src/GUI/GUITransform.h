#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "BoundingBox.h"
#include "GUICommon.h"

class GUITransform {
public:
	glm::vec3 position; //center
	glm::vec3 size;
	glm::quat rotation;
	GUITransform():position(0.0f),size(0.0f),rotation(glm::vec3(0.0f)){}
	GUITransform(const glm::vec2 position_,const glm::vec2 size_,const glm::quat rotation_):position(position_,0.0f),size(size_,0.0f),rotation(rotation_){}
	GUITransform(const glm::vec3 position_,const glm::vec3 size_,const glm::quat rotation_):position(position_),size(size_),rotation(rotation_){}
	explicit GUITransform(const BoundingBox& bb):position(glm::vec3(bb.GetCenter(),0.0f)),size(glm::vec3(bb.GetSize(),0.0f)),rotation(glm::vec3(0.0f)){}

	glm::mat4 ToMat4(const int layer) const {
		const glm::mat4 rotMat = (glm::translate(glm::mat4_cast(rotation),-glm::vec3(0.5f,0.5f,0.0f)));
		return glm::scale(glm::translate(glm::mat4(1.0f),glm::vec3(position.x,position.y,position.z/DEPTH_MAX+(float)layer)),size) * rotMat;
	}
	glm::mat4 ToMat4() const {
		return ToMat4(0);
	}

	bool ContainsPoint(const float pointX, const float pointY) const { //maybe dont work
		const glm::vec2 translatedPoint = glm::vec2(pointX,pointY) - GetMin();
		glm::vec2 rotatedPoint = glm::vec2(rotation * glm::vec3(translatedPoint,0.0f));
		return rotatedPoint.x >= 0 && rotatedPoint.x <= size.x && rotatedPoint.y >= 0 && rotatedPoint.y <= size.y;
	}
	bool ContainsPoint(const glm::vec2& point) const {
		const glm::vec2 translatedPoint = point - GetMin();
		glm::vec2 rotatedPoint = glm::vec2(rotation * glm::vec3(translatedPoint,0.0f));
		return rotatedPoint.x >= 0 && rotatedPoint.x <= size.x && rotatedPoint.y >= 0 && rotatedPoint.y <= size.y;
	}

	float GetX1() const {
		return position.x-size.x*0.5f;
	}
	float GetY1() const {
		return position.y-size.y*0.5f;
	}
	float GetX2() const {
		return position.x+size.x*0.5f;
	}
	float GetY2() const {
		return position.y+size.y*0.5f;
	}
	glm::vec2 GetMin() const {
		return position-size*0.5f;
	}
	glm::vec2 GetMax() const {
		return position+size*0.5f;
	}
	float GetHalfWidth() const {
		return size.x*0.5f;
	}
	float GetHalfHeight() const {
		return size.y*0.5f;
	}
	//float GetX2() const {
	//	return position.x+size.x;
	//}
	//float GetY2() const {
	//	return position.y+size.y;
	//}
	//glm::vec2 GetMax() const {
	//	return position+size;
	//}
	//glm::vec2 GetCenter() const {
	//	return position+size/2.0f;
	//}
	//glm::vec3 GetCenter3() const {
	//	return position+size/2.0f;
	//}


	BoundingBox CalculateEnclosingBoundingBox() const {
		const glm::mat4 model = ToMat4();

		const glm::vec2 bound1 = glm::vec2(model*glm::vec4(-0.5f,-0.5f,0.0f,0.0f));
		const glm::vec2 bound2 = glm::vec2(model*glm::vec4( 0.5f,-0.5f,0.0f,0.0f));
		const glm::vec2 bound3 = glm::vec2(model*glm::vec4(-0.5f, 0.5f,0.0f,0.0f));
		const glm::vec2 bound4 = glm::vec2(model*glm::vec4( 0.5f, 0.5f,0.0f,0.0f));

		const glm::vec2 minMaxX = minMaxFrom4(bound1.x,bound2.x,bound3.x,bound4.x)+position.x;
		const glm::vec2 minMaxY = minMaxFrom4(bound1.y,bound2.y,bound3.y,bound4.y)+position.y;

		return BoundingBox(minMaxX.x,minMaxY.x,minMaxX.y,minMaxY.y);
	}

	friend bool operator==(const GUITransform& lhs, const GUITransform& rhs) {
		return lhs.position==rhs.position&&lhs.size==rhs.size&&lhs.rotation==rhs.rotation;
	}
	friend bool operator!=(const GUITransform& lhs,const GUITransform& rhs) {
		return !(lhs==rhs);
	}
};