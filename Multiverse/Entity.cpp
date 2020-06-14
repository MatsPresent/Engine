#include "MultiversePCH.h"
#include "Entity.h"
#include "Universe.h"
#include "Component.h"


template <mv::uint dims>
mv::Entity<dims>::Entity(id_type id, id_type universe_id, const transform_type& transform, bool is_static)
	: _id{ id }, _universe_id{ universe_id },
	_transform_buffer{ transform }, _transform{ transform }, _velocity{},
	_component_ids{}, _is_static{ is_static }

{}

template <mv::uint dims>
mv::Entity<dims>::Entity(Entity&& other) noexcept
	: _id{ other._id }, _universe_id{ other._universe_id },
	_transform_buffer{ other._transform_buffer }, _transform{ other._transform }, _velocity{ other._velocity },
	_component_ids{ std::move(other._component_ids) }, _is_static{ other._is_static }
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
	this->_transform_buffer = other._transform_buffer;
	this->_transform = other._transform;
	this->_velocity = other._velocity;
	this->_component_ids = std::move(other._component_ids);
	this->_is_static = other._is_static;
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
	return this->universe()._transform_read_buffer ? this->_transform_buffer : this->_transform;
}

template <mv::uint dims>
const typename mv::Entity<dims>::transform_type& mv::Entity<dims>::get_velocity() const
{
	return this->_velocity;
}

template <mv::uint dims>
void mv::Entity<dims>::set_transform(const transform_type& transform)
{
	if (this->_is_static) {
		throw std::runtime_error("Entity::set_transform: static entities cannot be moved");
	}
	if (this->universe()._transform_readonly) {
		throw std::runtime_error("Entity::set_transform: transform is currently readonly");
	}
	this->_transform = transform;
}

template <mv::uint dims>
void mv::Entity<dims>::set_velocity(const transform_type& velocity)
{
	this->_velocity = velocity;
}


template <mv::uint dims>
bool mv::Entity<dims>::is_static() const
{
	return this->_is_static;
}


template <mv::uint dims>
void mv::Entity<dims>::add_collider(const Collider<dims>& collider)
{
	this->_colliders.push_back(collider);
}

template <mv::uint dims>
void mv::Entity<dims>::add_collider(Collider<dims>&& collider)
{
	this->_colliders.push_back(std::move(collider));
}


template <mv::uint dims>
void mv::Entity<dims>::_solve_collision(Entity<dims>& other)
{
	if (this->_colliders.empty() || other._colliders.empty()) {
		return;
	}

	for (std::size_t i = 0; i < this->_colliders.size(); ++i) {
		auto ta = this->_transform_buffer.transform_matrix();
		for (std::size_t j = 0; j < other._colliders.size(); ++j) {
			auto tb = other._transform_buffer.transform_matrix();
			Collider<dims>& a = this->_colliders[i];
			Collider<dims>& b = other._colliders[j];
			CollisionResponse response_ab = a.response(b.layer());
			CollisionResponse response_ba = b.response(a.layer());
			if (response_ab == CollisionResponse::ignore || response_ba == CollisionResponse::ignore) {
				continue;
			}
			if (response_ab == CollisionResponse::block && response_ba == CollisionResponse::block) {
				position_type mtv;
				a._shape.collides(b._shape, ta, tb, mtv);
				if (!other.is_static()) {
					mtv *= 0.5f;
					other._transform.translate -= mtv;
				}
				this->_transform.translate += mtv;
			}
			else {
				if (response_ab == CollisionResponse::overlap) {
					a._overlaps.insert(other.id());
				}
				if (response_ba == CollisionResponse::overlap) {
					b._overlaps.insert(this->id());
				}
			}
		}
	}
}


template class mv::Entity<2>;
template class mv::Entity<3>;
