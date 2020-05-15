#pragma once
#pragma warning(push)
#pragma warning(disable : 4201)
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#pragma warning(pop)
#include <type_traits>

namespace mv
{
	// behold my unholy creation

	template <unsigned int rows, unsigned int cols = rows>
	using Matrix = typename std::conditional<rows == 2,
		typename std::conditional<cols == 2, glm::mat2x2,
		typename std::conditional<cols == 3, glm::mat2x3,
		typename std::enable_if<cols == 4, glm::mat2x4>::type>::type>::type,
		typename std::conditional<rows == 3,
		typename std::conditional<cols == 2, glm::mat3x2,
		typename std::conditional<cols == 3, glm::mat3x3,
		typename std::enable_if<cols == 4, glm::mat3x4>::type>::type>::type,
		typename std::enable_if<rows == 4,
		typename std::conditional<cols == 2, glm::mat4x2,
		typename std::conditional<cols == 3, glm::mat4x3,
		typename std::enable_if<cols == 4, glm::mat4x4>::type>::type>::type>::type>::type>::type;
}
