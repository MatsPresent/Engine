#include "Level.h"

mv::BinaryReader& operator>>(mv::BinaryReader& br, Level& level)
{
	for (mv::size_type i = 0; i < (level._width * level._height) / 8; ++i) {
		char byte;
		br >> byte;
		for (mv::size_type j = 0; j < 8; ++j) {
			//level._cells[8 * i + j] = byte & (1 << j);
			level._cells[8 * i + j] = byte & (1 << (7 - j));
		}
	}
	return br;
}

std::vector<mv::vec2i> Level::block_positions() const
{
	std::vector<mv::vec2i> retval;

	for (mv::size_type y = 0; y < _height; y++) {
		for (mv::size_type x = 0; x < _width; x++) {
			if (this->_cells[x + _width * y]) {
				retval.push_back({ x, y });
			}
		}
	}

	return retval;
}
