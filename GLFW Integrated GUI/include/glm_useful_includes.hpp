#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/io.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

namespace glm
{
	template<typename T,qualifier Q>
	GLM_FUNC_QUALIFIER mat<4,4,T,Q> rotateAroundPivot(const mat<4,4,T,Q>& m, T angle, const vec<3,T,Q>& axis, const vec<3,T,Q>& p)
	{
		const qua<T,Q> q = rotate(quat_identity<T,Q>(), angle, axis);
		const vec<3,T,Q> offset= (p+1.0f)*0.5f; //idk
		const mat<4,4,T,Q> rotMat = translate(mat4_cast(q), -offset);
		const mat<4,4,T,Q> tBackMat = translate(mat4(1.0f), offset);
		return tBackMat*(m*rotMat);
	}

	template<typename T,qualifier Q>
	GLM_FUNC_QUALIFIER mat<4,4,T,Q> rotateAroundPivot(const mat<4,4,T,Q>& m, const qua<T,Q>& q, const vec<3,T,Q>& p)
	{
		const vec<3,T,Q> offset= (p+1.0f)*0.5f; //idk
		const mat<4,4,T,Q> rotMat = translate(mat4_cast(q), -offset);
		const mat<4,4,T,Q> tBackMat = translate(mat4(1.0f), offset);

		return tBackMat*(m*rotMat);
	}

	template<typename T,qualifier Q>
	GLM_FUNC_QUALIFIER mat<4,4,T,Q> rotateAroundCenter(const mat<4,4,T,Q>& m, T angle, const vec<3,T,Q>& axis)
	{
		const qua<T,Q> q = rotate(quat_identity<T,Q>(), angle, axis);
		constexpr vec<3,T,Q> offset= vec3(0.5f);
		const mat<4,4,T,Q> rotMat = translate(mat4_cast(q), -offset);
		const mat<4,4,T,Q> tBackMat = translate(mat4(1.0f), offset);

		return tBackMat*(m*rotMat);
	}

	template<typename T,qualifier Q>
	GLM_FUNC_QUALIFIER mat<4,4,T,Q> rotateAroundCenter(const mat<4,4,T,Q>& m, const qua<T,Q>& q)
	{
		constexpr vec<3,T,Q> offset= vec3(0.5f);
		const mat<4,4,T,Q> rotMat = translate(mat4_cast(q), -offset);
		const mat<4,4,T,Q> tBackMat = translate(mat4(1.0f), offset);

		return tBackMat*(m*rotMat);
	}
}