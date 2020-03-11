#include "MultiversePCH.h"
#include "Texture2D.h"
#include <SDL.h>

mv::Texture2D::~Texture2D()
{
	SDL_DestroyTexture(m_Texture);
}

SDL_Texture* mv::Texture2D::GetSDLTexture() const
{
	return m_Texture;
}

mv::Texture2D::Texture2D(SDL_Texture* texture)
{
	m_Texture = texture;
}
