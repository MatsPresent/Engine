#include "Universe.h"

template <mv::uint dims>
template <typename ComponentType>
inline mv::type_id_type mv::Universe<dims>::ComponentUpdater<ComponentType>::type_id() const
{
	return mv::type_id<ComponentType>();
}

template <mv::uint dims>
template <typename ComponentType>
inline std::size_t mv::Universe<dims>::ComponentUpdater<ComponentType>::size() const
{
	return this->_components.size();
}

template <mv::uint dims>
template <typename ComponentType>
inline mv::Component<dims, ComponentType::update_stage>& mv::Universe<dims>::ComponentUpdater<ComponentType>::at(std::size_t i)
{
	return this->_components.at(i);
}

template <mv::uint dims>
template <typename ComponentType>
inline const mv::Component<dims, ComponentType::update_stage>& mv::Universe<dims>::ComponentUpdater<ComponentType>::at(std::size_t i) const
{
	return this->_components.at(i);
}

template <mv::uint dims>
template <typename ComponentType>
inline ComponentType& mv::Universe<dims>::ComponentUpdater<ComponentType>::get(id_type id)
{
	return this->_components.at(_lookup.at(id));
}

template <mv::uint dims>
template <typename ComponentType>
inline const ComponentType& mv::Universe<dims>::ComponentUpdater<ComponentType>::get(id_type id) const
{
	return this->_components.at(_lookup.at(id));
}

template <mv::uint dims>
template <typename ComponentType>
inline ComponentType& mv::Universe<dims>::ComponentUpdater<ComponentType>::add(ComponentType&& component)
{
	id_type id;
	if (_freed_ids.empty()) {
		id = _lookup.size();
	}
	else {
		id = _freed_ids.back();
		_freed_ids.pop_back();
	}
	_lookup[id] = this->_components.size();
	this->_components.push_back(std::move(component));
	this->_components.back()._id = id;
	return this->_components.back();
}

template <mv::uint dims>
template <typename ComponentType>
inline void mv::Universe<dims>::ComponentUpdater<ComponentType>::remove(id_type id)
{
	this->_components.at(_lookup.at(id)) = std::move(this->_components.back());
	this->_components.pop_back();
	_lookup.at(this->_components.at(_lookup.at(id)).id()) = _lookup.at(id); // update index of moved component
	_freed_ids.push_back(id);
}

template <mv::uint dims>
template <typename ComponentType>
inline void mv::Universe<dims>::ComponentUpdater<ComponentType>::update(float deltaTime)
{
	for (ComponentType& component : this->_components) {
		component.update(deltaTime);
	}
}

template <mv::uint dims>
template <typename ComponentType>
inline void mv::Universe<dims>::ComponentUpdater<ComponentType>::render() const
{
	for (ComponentType& component : this->_components) {
		component.render();
	}
}



template <mv::uint dims>
template <mv::UpdateStage stage>
template <typename ComponentType>
inline ComponentType& mv::Universe<dims>::ComponentUpdaterList<stage>::get(id_type component_id) const
{
	return reinterpret_cast<ComponentUpdater<ComponentType>*>(nullptr);
	//return reinterpret_cast<ComponentUpdater<ComponentType>*>(this->_updaters.at(this->_lookup.at(type_id<ComponentType>()))->get(component_id);
}


template <mv::uint dims>
template <mv::UpdateStage stage>
template <typename ComponentType>
inline ComponentType& mv::Universe<dims>::ComponentUpdaterList<stage>::add(ComponentType&& component)
{
	auto it = this->_lookup.find(type_id<ComponentType>());
	if (it == this->_lookup.end()) {
		it = this->_lookup.emplace(type_id<ComponentType>(), this->_updaters.size()).first;
		this->_updaters.push_back(new ComponentUpdater<ComponentType>);
	}
	return reinterpret_cast<ComponentUpdater<ComponentType>*>(this->_updaters.at(it->second))->add(std::move(component));
}




template <mv::uint dims>
template <typename ComponentType, typename std::enable_if<std::is_base_of<mv::Component<dims, mv::UpdateStage::behaviour>, ComponentType>::value,int>::type>
inline ComponentType& mv::Universe<dims>::get_component(mv::id_type component_id) const
{
	return this->_behaviour_updaters.get<ComponentType>(component_id);
}

template <mv::uint dims>
template <typename ComponentType, typename std::enable_if<std::is_base_of<mv::Component<dims, mv::UpdateStage::prephysics>, ComponentType>::value,int>::type>
inline ComponentType& mv::Universe<dims>::get_component(mv::id_type component_id) const
{
	return this->_prephysics_updaters.get<ComponentType>(component_id);
}

template <mv::uint dims>
template <typename ComponentType, typename std::enable_if<std::is_base_of<mv::Component<dims, mv::UpdateStage::physics>, ComponentType>::value,int>::type>
inline ComponentType& mv::Universe<dims>::get_component(mv::id_type component_id) const
{
	return this->_physics_updaters.get<ComponentType>(component_id);
}

template <mv::uint dims>
template <typename ComponentType, typename std::enable_if<std::is_base_of<mv::Component<dims, mv::UpdateStage::postphysics>, ComponentType>::value,int>::type>
inline ComponentType& mv::Universe<dims>::get_component(mv::id_type component_id) const
{
	return this->_postphysics_updaters.get<ComponentType>(component_id);
}

template <mv::uint dims>
template <typename ComponentType, typename std::enable_if<std::is_base_of<mv::Component<dims, mv::UpdateStage::prerender>, ComponentType>::value,int>::type>
inline ComponentType& mv::Universe<dims>::get_component(mv::id_type component_id) const
{
	return this->_prerender_updaters.get<ComponentType>(component_id);
}

template <mv::uint dims>
template <typename ComponentType, typename std::enable_if<std::is_base_of<mv::Component<dims, mv::UpdateStage::render>, ComponentType>::value,int>::type>
inline ComponentType& mv::Universe<dims>::get_component(mv::id_type component_id) const
{
	return this->_render_updaters.get<ComponentType>(component_id);
}


template <mv::uint dims>
template <typename ComponentType, typename std::enable_if<std::is_base_of<mv::Component<dims, mv::UpdateStage::behaviour>, ComponentType>::value,int>::type>
inline ComponentType& mv::Universe<dims>::add_component(ComponentType&& component)
{
	return this->_behaviour_updaters.add(std::move(component));
}

template <mv::uint dims>
template <typename ComponentType, typename std::enable_if<std::is_base_of<mv::Component<dims, mv::UpdateStage::prephysics>, ComponentType>::value,int>::type>
inline ComponentType& mv::Universe<dims>::add_component(ComponentType&& component)
{
	return this->_prephysics_updaters.add(std::move(component));
}

template <mv::uint dims>
template <typename ComponentType, typename std::enable_if<std::is_base_of<mv::Component<dims, mv::UpdateStage::physics>, ComponentType>::value,int>::type>
inline ComponentType& mv::Universe<dims>::add_component(ComponentType&& component)
{
	return this->_physics_updaters.add(std::move(component));
}

template <mv::uint dims>
template <typename ComponentType, typename std::enable_if<std::is_base_of<mv::Component<dims, mv::UpdateStage::postphysics>, ComponentType>::value,int>::type>
inline ComponentType& mv::Universe<dims>::add_component(ComponentType&& component)
{
	return this->_postphysics_updaters.add(std::move(component));
}

template <mv::uint dims>
template <typename ComponentType, typename std::enable_if<std::is_base_of<mv::Component<dims, mv::UpdateStage::prerender>, ComponentType>::value,int>::type>
inline ComponentType& mv::Universe<dims>::add_component(ComponentType&& component)
{
	return this->_prerender_updaters.add(std::move(component));
}

template <mv::uint dims>
template <typename ComponentType, typename std::enable_if<std::is_base_of<mv::Component<dims, mv::UpdateStage::render>, ComponentType>::value,int>::type>
inline ComponentType& mv::Universe<dims>::add_component(ComponentType&& component)
{
	return this->_render_updaters.add(std::move(component));
}

template <mv::uint dims>
template <typename ComponentType, typename std::enable_if<std::is_base_of<mv::Component<dims, mv::UpdateStage::behaviour>, ComponentType>::value,int>::type>
inline void mv::Universe<dims>::remove_component(id_type component_id)
{
	this->_behaviour_updaters.remove(type_id<ComponentType>(), component_id);
}

template <mv::uint dims>
template <typename ComponentType, typename std::enable_if<std::is_base_of<mv::Component<dims, mv::UpdateStage::prephysics>, ComponentType>::value,int>::type>
inline void mv::Universe<dims>::remove_component(id_type component_id)
{
	this->_prephysics_updaters.remove(type_id<ComponentType>(), component_id);
}

template <mv::uint dims>
template <typename ComponentType, typename std::enable_if<std::is_base_of<mv::Component<dims, mv::UpdateStage::physics>, ComponentType>::value,int>::type>
inline void mv::Universe<dims>::remove_component(id_type component_id)
{
	this->_physics_updaters.remove(type_id<ComponentType>(), component_id);
}

template <mv::uint dims>
template <typename ComponentType, typename std::enable_if<std::is_base_of<mv::Component<dims, mv::UpdateStage::postphysics>, ComponentType>::value,int>::type>
inline void mv::Universe<dims>::remove_component(id_type component_id)
{
	this->_postphysics_updaters.remove(type_id<ComponentType>(), component_id);
}

template <mv::uint dims>
template <typename ComponentType, typename std::enable_if<std::is_base_of<mv::Component<dims, mv::UpdateStage::prerender>, ComponentType>::value,int>::type>
inline void mv::Universe<dims>::remove_component(id_type component_id)
{
	this->_prerender_updaters.remove(type_id<ComponentType>(), component_id);
}

template <mv::uint dims>
template <typename ComponentType, typename std::enable_if<std::is_base_of<mv::Component<dims, mv::UpdateStage::render>, ComponentType>::value,int>::type>
inline void mv::Universe<dims>::remove_component(id_type component_id)
{
	this->_render_updaters.remove(type_id<ComponentType>(), component_id);
}
