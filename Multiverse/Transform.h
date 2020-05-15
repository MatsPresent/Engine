#pragma once
#pragma warning(push)
#pragma warning(disable : 4201)
#include "glm/glm.hpp"
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
		glm::vec2 translate;
		float rotate;
		glm::vec2 scale;
	};

	template <>
	class Transform<3>
	{
	public:
		glm::vec3 translate;
		glm::quat rotate;
		glm::vec3 scale;
	};

	using Transform2D = Transform<2>;
	using Transform3D = Transform<3>;
}
