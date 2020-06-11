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
		vec2f translate;
		float rotate;
		vec2f scale;
	};

	template <>
	class Transform<3>
	{
	public:
		vec3f translate;
		glm::quat rotate;
		vec3f scale;
	};

	using Transform2D = Transform<2>;
	using Transform3D = Transform<3>;
}
