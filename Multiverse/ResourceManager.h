#pragma once
#include <string>
#include <unordered_map>

namespace mv
{
	class Multiverse;
	class Resource;

	class ResourceManager final
	{
		friend Multiverse;

	private:
		std::string _data_path;
		std::unordered_map<std::string, Resource*> _resources;


		ResourceManager(const std::string& data_path);
	public:
		ResourceManager(const ResourceManager&) = delete;
		ResourceManager(ResourceManager&& other) noexcept = default;

		~ResourceManager();

		ResourceManager& operator=(const ResourceManager&) = delete;
		ResourceManager& operator=(ResourceManager&& other) noexcept = default;

	private:
		void init();
		void register_resource(const std::string& path, const std::string& extension);

	public:
		template <typename T>
		const T* get(const std::string& id) const;
	};
}
