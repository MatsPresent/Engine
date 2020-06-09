#pragma once
#include "Resource.h"
#include "Vector.h"

struct SDL_Texture;

namespace mv
{
	class Texture final : public Resource
	{
	private:
		SDL_Texture* _texture;

	public:
		Texture(const std::string& path);
		Texture(const Texture&) = delete;
		Texture(Texture&& other) noexcept;

		~Texture();

		Texture& operator=(const Texture&) = delete;
		Texture& operator=(Texture&& other) noexcept;

		int width() const;
		int height() const;
		vec2i dims() const;

		void render(int x, int y, int w = 0, int h = 0) const;
		void render(vec2f translate, vec2f scale = { 1.f, 1.f }) const;
	};
}
