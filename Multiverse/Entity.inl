#include "Entity.h"
#include "Universe.h"
#include "Multiverse.h"


template <mv::uint dims>
template <typename ComponentType>
inline mv::Entity<dims>::ComponentIterator<ComponentType>::ComponentIterator(const id_type* component_id, id_type universe_id)
	: _component_id_it{ component_id }, _universe_id{ universe_id }
{}


template <mv::uint dims>
template <typename ComponentType>
inline bool mv::Entity<dims>::ComponentIterator<ComponentType>::operator==(const ComponentIterator<ComponentType>& rhs) const
{
	return this->_component_id_it == rhs._component_id_it;
}

template <mv::uint dims>
template <typename ComponentType>
inline bool mv::Entity<dims>::ComponentIterator<ComponentType>::operator!=(const ComponentIterator<ComponentType>& rhs) const
{
	return this->_component_id_it != rhs._component_id_it;
}

template <mv::uint dims>
template <typename ComponentType>
inline bool mv::Entity<dims>::ComponentIterator<ComponentType>::operator<(const ComponentIterator<ComponentType>& rhs) const
{
	return this->_component_id_it < rhs._component_id_it;
}

template <mv::uint dims>
template <typename ComponentType>
inline bool mv::Entity<dims>::ComponentIterator<ComponentType>::operator>(const ComponentIterator<ComponentType>& rhs) const
{
	return this->_component_id_it > rhs._component_id_it;
}

template <mv::uint dims>
template <typename ComponentType>
inline bool mv::Entity<dims>::ComponentIterator<ComponentType>::operator<=(const ComponentIterator<ComponentType>& rhs) const
{
	return this->_component_id_it <= rhs._component_id_it;
}

template <mv::uint dims>
template <typename ComponentType>
inline bool mv::Entity<dims>::ComponentIterator<ComponentType>::operator>=(const ComponentIterator<ComponentType>& rhs) const
{
	return this->_component_id_it >= rhs._component_id_it;
}


template <mv::uint dims>
template <typename ComponentType>
inline ComponentType& mv::Entity<dims>::ComponentIterator<ComponentType>::operator*() const
{
	return multiverse()._universe<dims>(this->_universe_id).get_component<ComponentType>(this->_component_id_it);
}

template <mv::uint dims>
template <typename ComponentType>
inline ComponentType* mv::Entity<dims>::ComponentIterator<ComponentType>::operator->() const
{
	return &this->operator*();
}


template <mv::uint dims>
template <typename ComponentType>
inline mv::Entity<dims>::ComponentIterator<ComponentType>& mv::Entity<dims>::ComponentIterator<ComponentType>::operator++()
{
	++this->_component_id_it;
	return *this;
}

template <mv::uint dims>
template <typename ComponentType>
inline mv::Entity<dims>::ComponentIterator<ComponentType> mv::Entity<dims>::ComponentIterator<ComponentType>::operator++(int)
{
	++this->_component_id_it;
	return ComponentIterator<ComponentType>(this->_component_id_it - 1, this->_universe_id);
}

template <mv::uint dims>
template <typename ComponentType>
inline mv::Entity<dims>::ComponentIterator<ComponentType>& mv::Entity<dims>::ComponentIterator<ComponentType>::operator--()
{
	--this->_component_id_it;
	return *this;
}

template <mv::uint dims>
template <typename ComponentType>
inline mv::Entity<dims>::ComponentIterator<ComponentType> mv::Entity<dims>::ComponentIterator<ComponentType>::operator--(int)
{
	--this->_component_id_it;
	return ComponentIterator<ComponentType>(this->_component_id_it + 1, this->_universe_id);
}


template <mv::uint dims>
template <typename ComponentType>
inline mv::Entity<dims>::ComponentIterator<ComponentType>& mv::Entity<dims>::ComponentIterator<ComponentType>::operator+=(std::ptrdiff_t n)
{
	this->_component_id_it += n;
	return *this;
}

template <mv::uint dims>
template <typename ComponentType>
inline mv::Entity<dims>::ComponentIterator<ComponentType> mv::Entity<dims>::ComponentIterator<ComponentType>::operator+(std::ptrdiff_t n) const
{
	return ComponentIterator<ComponentType>(this->_component_id_it + n, this->_universe_id);
}

template <mv::uint dims>
template <typename ComponentType>
inline mv::Entity<dims>::ComponentIterator<ComponentType>& mv::Entity<dims>::ComponentIterator<ComponentType>::operator-=(std::ptrdiff_t n)
{
	this->_component_id_it -= n;
	return *this;
}

template <mv::uint dims>
template <typename ComponentType>
inline mv::Entity<dims>::ComponentIterator<ComponentType> mv::Entity<dims>::ComponentIterator<ComponentType>::operator-(std::ptrdiff_t n) const
{
	return ComponentIterator<ComponentType>(this->_component_id_it - n, this->_universe_id);
}

template <mv::uint dims>
template <typename ComponentType>
inline std::ptrdiff_t mv::Entity<dims>::ComponentIterator<ComponentType>::operator-(const ComponentIterator<ComponentType>& rhs) const
{
	return this->_component_id_it - rhs._component_id_it;
}


template <mv::uint dims>
template <typename ComponentType>
inline ComponentType& mv::Entity<dims>::ComponentIterator<ComponentType>::operator[](std::ptrdiff_t i) const
{
	return *(*this + i);
}




template <mv::uint dims>
template <typename ComponentType>
inline mv::type_id_type mv::Entity<dims>::ComponentList<ComponentType>::type_id() const
{
	return mv::type_id<ComponentType>();
}


template <mv::uint dims>
template <typename ComponentType>
inline mv::Entity<dims>::ComponentIterator<ComponentType> mv::Entity<dims>::ComponentList<ComponentType>::begin() const
{
	return ComponentIterator<ComponentType>(this->_component_ids, this->_universe_id);
}

template <mv::uint dims>
template <typename ComponentType>
inline mv::Entity<dims>::ComponentIterator<ComponentType> mv::Entity<dims>::ComponentList<ComponentType>::end() const
{
	return ComponentIterator<ComponentType>(this->_component_ids + this->_component_count, this->_universe_id);
}


template <mv::uint dims>
template <typename ComponentType>
inline mv::size_type mv::Entity<dims>::ComponentList<ComponentType>::size() const
{
	return this->_component_count;
}

template <mv::uint dims>
template <typename ComponentType>
inline bool mv::Entity<dims>::ComponentList<ComponentType>::empty() const
{
	return this->_component_count != 0u;
}


template <mv::uint dims>
template <typename ComponentType>
inline ComponentType& mv::Entity<dims>::ComponentList<ComponentType>::operator[](mv::size_type i) const
{
	return this->begin()[i];
}

template <mv::uint dims>
template <typename ComponentType>
inline ComponentType& mv::Entity<dims>::ComponentList<ComponentType>::at(mv::size_type i) const
{
	return this->begin()[i];
}




template <mv::uint dims>
template <typename ComponentType>
inline ComponentType& mv::Entity<dims>::component() const
{
	return this->_universe().get_component<ComponentType>(this->_component_ids.at(type_id<ComponentType>()).front());
}

template <mv::uint dims>
template <typename ComponentType>
inline mv::Entity<dims>::ComponentList<ComponentType> mv::Entity<dims>::components() const
{
	auto it = this->_component_ids.find(type_id<ComponentType>());
	bool found = it != this->_component_ids.cend();
	return ComponentList<ComponentType>(found ? it->second.data() : nullptr, found ? it->second.size() : 0, this->_universe_id);
}

template <mv::uint dims>
template <typename ComponentType>
inline ComponentType* mv::Entity<dims>::find_component() const
{
	auto it = this->_component_ids.find(type_id<ComponentType>());
	if (it == this->_component_ids.cend())
		return nullptr;
	return &this->_universe().get_component<ComponentType>(it->second.front());
}


template <mv::uint dims>
template <typename ComponentType, typename... Args>
inline ComponentType& mv::Entity<dims>::add_component(Args&&... args)
{
	ComponentType& component = this->_universe().add_component(ComponentType(std::forward<Args>(args)...));
	std::map<type_id_type, std::vector<id_type>>::iterator it = this->_component_ids.emplace(type_id<ComponentType>());
	it->second.push_back(component.id());
	return component;
}

template <mv::uint dims>
template <typename ComponentType>
inline bool mv::Entity<dims>::remove_component(id_type component_id)
{
	std::map<type_id_type, std::vector<id_type>>::iterator it = this->_component_ids.find(type_id<ComponentType>());
	for (std::size_t i = 0; i < it->second.size(); ++i) {
		if (it->second[i] == component_id) {
			it->second[i] == it->second.back();
			it->second.pop_back();
			if (it->second.empty()) {
				this->_component_ids.erase(it);
			}
			this->_universe().remove_component<ComponentType>(component_id);
			return true;
		}
	}
	return false;
}




template <mv::uint dims, typename ComponentType>
typename mv::Entity<dims>::template ComponentIterator<ComponentType> mv::operator+(
	std::ptrdiff_t n, const typename mv::Entity<dims>::template ComponentIterator<ComponentType>& it)
{
	return it + n;
}
