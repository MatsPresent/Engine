#pragma once
#include "Vector.h"
#pragma warning(push)
#pragma warning(disable : 4201)
#include "glm/gtc/quaternion.hpp"
#pragma warning(pop)

namespace mv
{
	template <unsigned int dims>
	class Transform;

	template <>
	class Transform<2>
	{
	public:
		Transform();

		vec2f translate;
		float rotate;
		vec2f scale;

		bool operator==(const Transform<2>& rhs) const;
		bool operator!=(const Transform<2>& rhs) const;
	};

	template <>
	class Transform<3>
	{
	public:
		Transform();

		vec3f translate;
		glm::quat rotate;
		vec3f scale;

		bool operator==(const Transform<3>& rhs) const;
		bool operator!=(const Transform<3>& rhs) const;
	};

	using Transform2D = Transform<2>;
	using Transform3D = Transform<3>;
}
