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
	GLM_FUNC_QUALIFIER mat<4,4,T,Q> rotateAroundPivot(mat<4,4,T,Q> const& m,T angle,vec<3,T,Q> const& v,vec<3,T,Q> const& p)
	{
		const qua<T,Q> q = rotate(quat_identity<T,Q>(), angle, v);

		const vec<3,T,Q> offset= (p+1.0f)*0.5f; //idk

		const mat<4,4,T,Q> rot = (glm::translate(glm::mat4_cast(q),-offset));
		const mat<4,4,T,Q> tBackMat = glm::translate(glm::mat4(1.0f),offset);
		return tBackMat*(m*rot);
	}

	template<typename T,qualifier Q>
	GLM_FUNC_QUALIFIER mat<4,4,T,Q> rotateAroundPivot(mat<4,4,T,Q> const& m,qua<T,Q> const& q,vec<3,T,Q> const& p)
	{
		const vec<3,T,Q> offset= (p+1.0f)*0.5f; //idk
		const mat<4,4,T,Q> rot = (glm::translate(glm::mat4_cast(q),-offset));
		const mat<4,4,T,Q> tBackMat = glm::translate(glm::mat4(1.0f),offset);

		return m*rot;
	}
}