#pragma once
#include "setup.h"
#include <string>

namespace mv
{
	class ResourceManager;

	class Resource
	{
		friend ResourceManager;

	protected:
		Resource() = default;
	public:
		virtual ~Resource() = default;
	};
}
