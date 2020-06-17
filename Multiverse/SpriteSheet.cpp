#include "MultiversePCH.h"
#include "SpriteSheet.h"

mv::SpriteSheet::SpriteSheet(const std::string& path)
	: Resource(path)
{}

mv::SpriteSheet::SpriteSheet(std::string&& path)
	: Resource(std::move(path))
{}


void mv::SpriteSheet::_load()
{
}

void mv::SpriteSheet::_unload()
{
}
