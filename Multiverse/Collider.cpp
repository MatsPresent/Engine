#include "MultiversePCH.h"
#include "Collider.h"

#include "Entity.h"


template <mv::uint dims>
void mv::Collider<dims>::set_shape(const CollisionShape<dims>& shape)
{
	this->_shape = shape;
}

template <mv::uint dims>
void mv::Collider<dims>::set_shape(CollisionShape<dims>&& shape)
{
	this->_shape = std::move(shape);
}

template <mv::uint dims>
void mv::Collider<dims>::set_layer(CollisionLayer layer)
{
	this->_layer = layer;
}

template <mv::uint dims>
void mv::Collider<dims>::set_response(CollisionLayer layer, CollisionResponse response)
{
	// clear response
	this->_response_mask &= ~static_cast<uint>(0b11u << (2u * static_cast<uint>(layer)));
	// set new response
	this->_response_mask |= static_cast<uint>((0b11u & static_cast<uint>(response)) << (2u * static_cast<uint>(layer)));
}


template <mv::uint dims>
mv::CollisionLayer mv::Collider<dims>::layer() const
{
	return this->_layer;
}

template <mv::uint dims>
mv::CollisionResponse mv::Collider<dims>::response(CollisionLayer layer) const
{
	return static_cast<CollisionResponse>((this->_response_mask >> (2u * static_cast<uint>(layer))) & 0b11u);
}


template <mv::uint dims>
const std::set<mv::id_type>& mv::Collider<dims>::overlaps() const
{
	return this->_overlaps;
}




template class mv::Collider<2>;
template class mv::Collider<3>;
