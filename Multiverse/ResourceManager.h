#pragma once
#include <string>
#include <unordered_map>

namespace mv
{
	class Resource;

	class ResourceManager final
	{
	private:
		std::string _data_path;
		std::unordered_map<std::string, Resource*> _resources;


	public:
		ResourceManager(const std::string& data_path);
		ResourceManager(const ResourceManager&) = delete;
		ResourceManager(ResourceManager&& other) noexcept;

		~ResourceManager();

		ResourceManager& operator=(const ResourceManager&) = delete;
		ResourceManager& operator=(ResourceManager&& other) noexcept;

	private:
		void _init();
		void _register_resource(const std::string& path, const std::string& extension);

	public:
		template <typename T>
		const T* get(const std::string& id) const;
	};
}
