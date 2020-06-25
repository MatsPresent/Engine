#pragma once
#include "Debug.h"

namespace mv
{
	class ConsoleLogger : public DebugService
	{
	public:
		void log(const std::string& message) override;
		void warning(const std::string& message) override;
		void error(const std::string& message) override;
		void verify(bool(*test)(), const std::string& message) override;
	};
}
