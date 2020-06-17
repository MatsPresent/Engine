#pragma once
#include "Resource.h"
#include "Vector.h"

struct SDL_Texture;

namespace mv
{
	class Texture final : public Resource
	{
	private:
		uint _texture_id;
		size_type _width;
		size_type _height;
		size_type _channel_count;

	public:
		Texture(const std::string& path);
		Texture(std::string&& path);

		size_type width() const;
		size_type height() const;

	private:
		void _load() override;
		void _unload() override;
	};
}
