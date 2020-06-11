#include "MultiversePCH.h"
#include "Entity.h"
#include "Universe.h"


template <mv::uint dims>
mv::Entity<dims>::Entity(id_type id, id_type universe_id, const transform_type& transform)
	: _id{ id }, _universe_id{ universe_id },
	_transform_read{ transform }, _transform_write{ transform }, _velocity{},
	_component_ids{}

{}

template <mv::uint dims>
mv::Entity<dims>::Entity(Entity&& other) noexcept
	: _id{ other._id }, _universe_id{ other._universe_id },
	_transform_read{ other._transform_read }, _transform_write{ other._transform_write }, _velocity{ other._velocity },
	_component_ids{ std::move(other._component_ids) }
{
	other._id = invalid_id;
	other._universe_id = invalid_id;
}


template <mv::uint dims>
mv::Entity<dims>& mv::Entity<dims>::operator=(Entity&& other) noexcept
{
	if (this == &other)
		return *this;
	this->_id = other._id;
	this->_universe_id = other._universe_id;
	this->_transform_read = other._transform_read;
	this->_transform_write = other._transform_write;
	this->_velocity = other._velocity;
	this->_component_ids = std::move(other._component_ids);
	other._id = invalid_id;
	other._universe_id = invalid_id;
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


template <mv::uint dims>
const typename mv::Entity<dims>::transform_type& mv::Entity<dims>::get_transform() const
{
	return this->_transform_read;
}

template <mv::uint dims>
const typename mv::Entity<dims>::transform_type& mv::Entity<dims>::get_velocity() const
{
	return this->_velocity;
}

template <mv::uint dims>
void mv::Entity<dims>::set_transform(const transform_type& transform)
{
	if (this->universe().is_transform_locked()) {
		throw std::runtime_error("Entity::set_transform: transform is currently readonly");
	}
	this->_transform_write = transform;
}

template <mv::uint dims>
void mv::Entity<dims>::set_velocity(const transform_type& velocity)
{
	this->_velocity = velocity;
}


template class mv::Entity<2>;
template class mv::Entity<3>;
