#include "MultiversePCH.h"
#include "Entity.h"
#include "Universe.h"


template <mv::uint dims>
mv::Entity<dims>::Entity(id_type id, id_type universe_id)
	: _id{ id }, _universe_id{ universe_id }
{}

template <mv::uint dims>
mv::Entity<dims>::Entity(Entity&& other) noexcept
	: _id{ other._id }, _universe_id{ other._universe_id }, _physics_id{ other._physics_id }, _physics_type_id{ other._physics_type_id },
	_component_ids{ std::move(other._component_ids) }
{
	other._id = invalid_id;
	other._universe_id = invalid_id;
	other._physics_id = invalid_id;
	other._physics_type_id = invalid_id;
}


template <mv::uint dims>
mv::Entity<dims>& mv::Entity<dims>::operator=(Entity&& other) noexcept
{
	if (this == &other)
		return *this;
	this->_id = other._id;
	this->_universe_id = other._universe_id;
	this->_physics_id = other._physics_id;
	this->_physics_type_id = other._physics_type_id;
	this->_component_ids = std::move(other._component_ids);
	other._id = invalid_id;
	other._universe_id = invalid_id;
	other._physics_id = invalid_id;
	other._physics_type_id = invalid_id;
	return *this;
}


template <mv::uint dims>
inline mv::id_type mv::Entity<dims>::id() const
{
	return this->_id;
}

template <mv::uint dims>
mv::id_type mv::Entity<dims>::universe_id() const
{
	return this->_universe_id;
}

template <mv::uint dims>
mv::Universe<dims>& mv::Entity<dims>::universe() const
{
	return mv::multiverse().universe<dims>(this->_universe_id);
}

template class mv::Entity<2>;
template class mv::Entity<3>;
