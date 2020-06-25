#pragma once
#include <string>

#include "ServiceProxy.h"

namespace mv
{
	class DebugService
	{
	public:
		virtual ~DebugService() {}

		virtual void log(const std::string& message) = 0;
		virtual void warning(const std::string& message) = 0;
		virtual void error(const std::string& message) = 0;
		virtual void verify(bool(*test)(), const std::string& message) = 0;
	};

	extern const ServiceProxy<DebugService> debug;
}
