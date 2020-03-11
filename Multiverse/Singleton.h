#pragma once
namespace mv
{
	template <typename T>
	class Singleton
	{
	public:
		static T& instance()
		{
			static T obj{};
			return obj;
		}

		~Singleton() = default;
		Singleton(const Singleton& other) = delete;
		Singleton(Singleton&& other) = delete;
		Singleton& operator=(const Singleton& other) = delete;
		Singleton& operator=(Singleton&& other) = delete;

	protected:
		Singleton() = default;
	};
}