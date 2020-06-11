#include "MultiversePCH.h"
#include "Component.h"

#include "Multiverse.h"
#include "Entity.h"


template <mv::uint dims, mv::UpdateStage stage>
mv::id_type mv::Component<dims, stage>::id() const
{
	return this->_id;
}

template <mv::uint dims, mv::UpdateStage stage>
mv::id_type mv::Component<dims, stage>::entity_id() const
{
	return this->_entity_id;
}

template <mv::uint dims, mv::UpdateStage stage>
mv::id_type mv::Component<dims, stage>::universe_id() const
{
	return this->entity().universe_id();
}

template <mv::uint dims, mv::UpdateStage stage>
mv::Entity<dims>& mv::Component<dims, stage>::entity() const
{
	return mv::multiverse().entity<dims>(this->_entity_id);
}

template <mv::uint dims, mv::UpdateStage stage>
mv::Universe<dims>& mv::Component<dims, stage>::universe() const
{
	return this->entity().universe();
}


template <mv::uint dims, mv::UpdateStage stage>
void mv::Component<dims, stage>::update(float)
{}




template <mv::uint dims>
mv::id_type mv::Component<dims, mv::UpdateStage::render>::id() const
{
	return this->_id;
}

template <mv::uint dims>
mv::id_type mv::Component<dims, mv::UpdateStage::render>::entity_id() const
{
	return this->_entity_id;
}

template <mv::uint dims>
mv::id_type mv::Component<dims, mv::UpdateStage::render>::universe_id() const
{
	return this->entity().universe_id();
}

template <mv::uint dims>
mv::Entity<dims>& mv::Component<dims, mv::UpdateStage::render>::entity() const
{
	return mv::multiverse().entity<dims>(this->_entity_id);
}

template <mv::uint dims>
mv::Universe<dims>& mv::Component<dims, mv::UpdateStage::render>::universe() const
{
	return this->entity().universe();
}


template <mv::uint dims>
void mv::Component<dims, mv::UpdateStage::render>::update(float)
{}

template <mv::uint dims>
void mv::Component<dims, mv::UpdateStage::render>::render() const
{}



template class mv::Component<2, mv::UpdateStage::behaviour>;
template class mv::Component<2, mv::UpdateStage::prephysics>;
template class mv::Component<2, mv::UpdateStage::physics>;
template class mv::Component<2, mv::UpdateStage::postphysics>;
template class mv::Component<2, mv::UpdateStage::prerender>;
template class mv::Component<2, mv::UpdateStage::render>;
template class mv::Component<3, mv::UpdateStage::behaviour>;
template class mv::Component<3, mv::UpdateStage::prephysics>;
template class mv::Component<3, mv::UpdateStage::physics>;
template class mv::Component<3, mv::UpdateStage::postphysics>;
template class mv::Component<3, mv::UpdateStage::prerender>;
template class mv::Component<3, mv::UpdateStage::render>;
