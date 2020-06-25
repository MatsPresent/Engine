#include "Multiverse.h"
#include "ResourceManager.h"
#include "Transform.h"

#include <memory>
#include <iostream>
#include "Universe.h"
#include "Entity.h"
#include "SpriteRenderComponent.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "Level.h"

#include <vector>
#include <vld.h>

int main(int, char**)
{
	mv::Multiverse::init();
	mv::Universe2D& universe = mv::Multiverse::create_universe<2>();
	auto& e = universe.spawn_entity();
	e.add_component<mv::SpriteRenderComponent>(mv::Multiverse::resource_manager().get<mv::Texture>("sprites0.png"));
	auto t = e.get_transform();
	t.translate = { 40.f, 70.f };
	e.set_transform(t);

	mv::BinaryReader level_reader(mv::Multiverse::resource_manager().get<mv::Blob>("leveldata.dat"));
	std::vector<Level> levels(100);
	for (mv::size_type i = 0; i < 100; ++i) {
		level_reader >> levels[i];
	}
	auto block_positions = levels[0].block_positions();

	mv::Multiverse::run();
	return 0;
}
