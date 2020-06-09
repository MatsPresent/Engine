#include "MultiversePCH.h"
#include "Texture.h"
#include "Renderer.h"

#include <SDL_image.h>


mv::Texture::Texture(const std::string& path)
	: Resource(), _texture{ IMG_LoadTexture(Renderer::instance().GetSDLRenderer(), path.data()) }
{
	if (!this->_texture) {
		throw std::runtime_error(std::string("Failed to load texture: ") + SDL_GetError());
	}
}

mv::Texture::Texture(Texture&& other) noexcept
	: Resource(std::move(other)), _texture{ other._texture }
{
	other._texture = nullptr;
}


mv::Texture::~Texture()
{
	SDL_DestroyTexture(this->_texture);
}


mv::Texture& mv::Texture::operator=(Texture&& other) noexcept
{
	if (this == &other) {
		return *this;
	}
	this->Resource::operator=(std::move(other));
	this->_texture = other._texture;
	other._texture = nullptr;
	return *this;
}


int mv::Texture::width() const
{
	int width;
	SDL_QueryTexture(this->_texture, nullptr, nullptr, &width, nullptr);
	return width;
}

int mv::Texture::height() const
{
	int height;
	SDL_QueryTexture(this->_texture, nullptr, nullptr, nullptr, &height);
	return height;
}

mv::vec2i mv::Texture::dims() const
{
	vec2i dims;
	SDL_QueryTexture(this->_texture, nullptr, nullptr, &dims.x(), &dims.y());
	return dims;
}

void mv::Texture::render(int x, int y, int w, int h) const
{
	SDL_Rect dst{ x, y, w, h };
	SDL_RenderCopy(Renderer::instance().GetSDLRenderer(), this->_texture, nullptr, &dst);
}

void mv::Texture::render(vec2f translate, vec2f scale) const
{
	vec2i dims = this->dims();
	this->render(static_cast<int>(translate.x()), static_cast<int>(translate.y()),
		static_cast<int>(dims.x() * scale.x()), static_cast<int>(dims.y() * scale.y()));
}
