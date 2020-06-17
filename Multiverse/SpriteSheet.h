#pragma once
#include "Resource.h"

#include <vector>

#include "Texture.h"
#include "Vector.h"

namespace mv
{
	class SpriteSheet final : public Resource
	{
	private:
		struct TexData
		{
			const Texture* texture;
			uint row_count;
			uint row_gutter;
			uint col_count;
			uint col_gutter;
			uint sprite_count;
			uint sprite_width;
			uint sprite_height;
		};
		struct AnimData
		{
			uint start_sprite;
			uint frame_count;
			float duration;
		};

		std::vector<TexData> _tex_data;
		std::vector<AnimData> _anim_data;

	public:
		SpriteSheet(const std::string& path);
		SpriteSheet(std::string&& path);

	private:
		void _load() override;
		void _unload() override;
	};
}
