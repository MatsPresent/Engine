#pragma once
#include "Multiverse.h"

#include <vector>

#include "BinaryReader.h"
#include "Vector.h"

class Level
{
private:
	constexpr static mv::size_type _width = 32;
	constexpr static mv::size_type _height = 25;

	bool _cells[_width * _height];

	friend mv::BinaryReader& operator>>(mv::BinaryReader& br, Level& level);

public:
	std::vector<mv::vec2i> block_positions() const;
};

mv::BinaryReader& operator>>(mv::BinaryReader& br, Level& level);
