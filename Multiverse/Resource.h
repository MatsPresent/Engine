#pragma once
#include "setup.h"
#include <string>

namespace mv
{
	class ResourceManager;

	class Resource
	{
		friend ResourceManager;

	private:
		std::string _path;
		bool _loaded;

	protected:
		Resource(const std::string& path);
		Resource(std::string&& path);
		Resource(const Resource&) = delete;
		Resource(Resource&&) = delete;
	public:
		virtual ~Resource();

		Resource& operator=(const Resource&) = delete;
		Resource& operator=(Resource&&) = delete;

		const std::string& path() const;

		bool loaded() const;

	protected:
		void load();
		void unload();

	private:
		virtual void _load() = 0;
		virtual void _unload() = 0;
	};
}
