#include "MultiversePCH.h"
#include "SpriteRenderComponent.h"

#include "Texture.h"
#include "Entity.h"

mv::SpriteRenderComponent::SpriteRenderComponent(const Texture* texture)
	: _texture{ texture }
{}

void mv::SpriteRenderComponent::update(float)
{
}

void mv::SpriteRenderComponent::render() const
{
	this->_texture->render(this->entity().get_transform().translate, this->entity().get_transform().scale);
}
