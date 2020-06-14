#include "CollisionShape.h"


template <typename InputIterator>
mv::CollisionShape<2>::Convex::Convex(InputIterator first, InputIterator last)
	: _vertices{ new vec2f[last - first]{} }, _vertex_count{ last - first }
{
	for (unsigned int i{ 0 }; first != last; ++i, ++first) {
		this->_vertices[i] = *first;
	}
}
