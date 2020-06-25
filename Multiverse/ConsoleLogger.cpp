#include "MultiversePCH.h"
#include "ConsoleLogger.h"

void mv::ConsoleLogger::log(const std::string& message)
{
	std::cout << message << std::endl;
}

void mv::ConsoleLogger::warning(const std::string& message)
{
#ifdef _WIN32
	HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	_CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(console_handle, &info);
	SetConsoleTextAttribute(console_handle, 14);
#endif
	std::cout << message << std::endl;
#ifdef _WIN32
	SetConsoleTextAttribute(console_handle, info.wAttributes);
#endif
}

void mv::ConsoleLogger::error(const std::string& message)
{
#ifdef _WIN32
	HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	_CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(console_handle, &info);
	SetConsoleTextAttribute(console_handle, 12);
#endif
	std::cout << message << std::endl;
#ifdef _WIN32
	SetConsoleTextAttribute(console_handle, info.wAttributes);
#endif
}

void mv::ConsoleLogger::verify(bool(*test)(), const std::string& message)
{
#ifdef _WIN32
	HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	_CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(console_handle, &info);
#endif
	if (test()) {
#ifdef _WIN32
		SetConsoleTextAttribute(console_handle, 12);
#endif
		std::cout << "[SUCCESS] ";
	}
	else {
#ifdef _WIN32
		SetConsoleTextAttribute(console_handle, 10);
#endif
		std::cout << "[FAILURE] ";
	}
	#ifdef _WIN32
		SetConsoleTextAttribute(console_handle, info.wAttributes);
	#endif

	std::cout << message << std::endl;
}
