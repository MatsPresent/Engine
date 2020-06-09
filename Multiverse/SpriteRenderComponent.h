#pragma once
#include "Component.h"

namespace mv
{
	class Texture;

	class SpriteRenderComponent : public Component<2, UpdateStage::render>
	{
	private:
		const Texture* _texture;

	public:
		SpriteRenderComponent(const Texture* texture);

		void update(float delta_time);
		void render() const;
	};
}
