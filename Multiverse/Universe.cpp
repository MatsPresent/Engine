#include "MultiversePCH.h"
#include "Universe.h"

#include "Multiverse.h"

template <mv::uint dims>
template <mv::UpdateStage stage>
mv::Universe<dims>::ComponentUpdaterList<stage>::ComponentUpdaterList(ComponentUpdaterList<stage>&& other) noexcept
	: _updaters{ std::move(other._updaters) }, _lookup{ std::move(other._lookup) }
{}


template <mv::uint dims>
template <mv::UpdateStage stage>
mv::Universe<dims>::ComponentUpdaterList<stage>::~ComponentUpdaterList()
{
	for (ComponentUpdaterBase<stage>* updater : this->_updaters) {
		delete updater;
	}
	this->_updaters.clear();
}


template <mv::uint dims>
template <mv::UpdateStage stage>
mv::Universe<dims>::ComponentUpdaterList<stage>& mv::Universe<dims>::ComponentUpdaterList<stage>::operator=(ComponentUpdaterList<stage>&& other) noexcept
{
	if (this == &other)
		return *this;
	this->_updaters = std::move(other._updaters);
	this->_lookup = std::move(other._lookup);
	return *this;
}


template <mv::uint dims>
template <mv::UpdateStage stage>
mv::Universe<dims>::ComponentUpdaterBase<stage>* const* mv::Universe<dims>::ComponentUpdaterList<stage>::begin() const
{
	return this->_updaters.data();
}

template <mv::uint dims>
template <mv::UpdateStage stage>
mv::Universe<dims>::ComponentUpdaterBase<stage>* const* mv::Universe<dims>::ComponentUpdaterList<stage>::end() const
{
	return this->_updaters.data() + this->_updaters.size();
}

template <mv::uint dims>
template <mv::UpdateStage stage>
void mv::Universe<dims>::ComponentUpdaterList<stage>::remove(type_id_type component_type_id, id_type component_id)
{
	this->_updaters.at(this->_lookup.at(component_type_id))->remove(component_id);
}




template <mv::uint dims>
template <mv::uint _, typename std::enable_if<_ == 2, int>::type>
mv::Universe<dims>::Gridspace::Gridspace(
	mv::uint cell_count_x, mv::uint cell_count_y, float cell_size_x, float cell_size_y)
	: _cells{ new std::vector<id_type>[cell_count_x * cell_count_y] },
	_cell_counts{ cell_count_x, cell_count_y }, _cell_sizes{ cell_size_x, cell_size_y }
{}

template <mv::uint dims>
template <mv::uint _, typename std::enable_if<_ == 3, int>::type>
mv::Universe<dims>::Gridspace::Gridspace(
	mv::uint cell_count_x, mv::uint cell_count_y, mv::uint cell_count_z, float cell_size_x, float cell_size_y, float cell_size_z)
	: _cells{ new std::vector<id_type>[cell_count_x * cell_count_y * cell_count_z] },
	_cell_counts{ cell_count_x, cell_count_y, cell_count_z }, _cell_sizes{ cell_size_x, cell_size_y, cell_size_y }
{}

template <mv::uint dims>
inline mv::Universe<dims>::Gridspace::Gridspace(const Gridspace& other)
	: _cells{ new std::vector<id_type>[other._cell_count()] }, _cell_counts{}, _cell_sizes{}
{
	for (uint i = 0; i < dims; ++i) {
		this->_cell_counts[i] = other._cell_counts[i];
		this->_cell_sizes[i] = other._cell_sizes[i];
	}
	for (uint i = 0; i < this->_cell_count(); ++i) {
		this->_cells[i] = other._cells[i];
	}
}

template <mv::uint dims>
mv::Universe<dims>::Gridspace::Gridspace(Gridspace&& other) noexcept
	: _cells{ other._cells }, _cell_counts{}, _cell_sizes{}
{
	other._cells = nullptr;
	for (uint i = 0; i < dims; ++i) {
		this->_cell_counts[i] = other._cell_counts[i];
		this->_cell_sizes[i] = other._cell_sizes[i];
	}
}


template <mv::uint dims>
mv::Universe<dims>::Gridspace::~Gridspace()
{
	delete[] this->_cells;
	this->_cells = nullptr;
}


template <mv::uint dims>
typename mv::Universe<dims>::Gridspace& mv::Universe<dims>::Gridspace::operator=(const Gridspace& other)
{
	if (this == &other)
		return *this;

	this->_cells = new std::vector<id_type>[other._cell_count()];
	for (uint i = 0; i < dims; ++i) {
		this->_cell_counts[i] = other._cell_counts[i];
		this->_cell_sizes[i] = other._cell_sizes[i];
	}
	for (uint i = 0; i < this->_cell_count(); ++i) {
		this->_cells[i] = other._cells[i];
	}

	return *this;
}

template <mv::uint dims>
typename mv::Universe<dims>::Gridspace& mv::Universe<dims>::Gridspace::operator=(Gridspace&& other) noexcept
{
	if (this == &other)
		return *this;

	this->_cells = other._cells;
	other._cells = nullptr;
	for (uint i = 0; i < dims; ++i) {
		this->_cell_counts[i] = other._cell_counts[i];
		this->_cell_sizes[i] = other._cell_sizes[i];
	}

	return *this;
}


template <mv::uint dims>
template <mv::uint _, typename std::enable_if<_ == 2, int>::type>
void mv::Universe<dims>::Gridspace::update_cells()
{

}

template <mv::uint dims>
template <mv::uint _, typename std::enable_if<_ == 3, int>::type>
void mv::Universe<dims>::Gridspace::update_cells()
{

}


template <mv::uint dims>
template <mv::uint _, typename std::enable_if<_ == 2, int>::type>
std::vector<mv::Entity<2>*> mv::Universe<dims>::Gridspace::entities_in_range(const position_type& origin, float radius) const
{

	return {};
}

template <mv::uint dims>
template <mv::uint _, typename std::enable_if<_ == 3, int>::type>
std::vector<mv::Entity<3>*> mv::Universe<dims>::Gridspace::entities_in_range(const position_type& origin, float radius) const
{

	return {};
}


template <mv::uint dims>
template <mv::uint _, typename std::enable_if<_ == 2, int>::type>
inline mv::uint mv::Universe<dims>::Gridspace::_cell_count() const
{
	return this->_cell_counts[0] * this->_cell_counts[1];
}

template <mv::uint dims>
template <mv::uint _, typename std::enable_if<_ == 3, int>::type>
inline mv::uint mv::Universe<dims>::Gridspace::_cell_count() const
{
	return this->_cell_counts[0] * this->_cell_counts[1] * this->_cell_counts[2];
}




template <mv::uint dims>
template <mv::uint _, typename std::enable_if<_ == 2, int>::type>
mv::Universe<dims>::Universe(
	id_type id, uint cell_count_x, uint cell_count_y, float cell_size_x, float cell_size_y)
	: _id{ id }, _gridspace(cell_count_x, cell_count_y, cell_size_x, cell_size_y),
	_behaviour_updaters{}, _prephysics_updaters{}, _physics_updaters{},
	_postphysics_updaters{}, _prerender_updaters{}, _render_updaters{},
	_update_interval{ 0.f }, _update_timeout{ 0.f }, _render_interval{ 0.f }, _render_timeout{ 0.f },
	_update_enabled{ true }, _render_enabled{ true }
{}

template <mv::uint dims>
template <mv::uint _, typename std::enable_if<_ == 3, int>::type>
mv::Universe<dims>::Universe(
	id_type id, uint cell_count_x, uint cell_count_y, uint cell_count_z, float cell_size_x, float cell_size_y, float cell_size_z)
	: _id{ id }, _gridspace(cell_count_x, cell_count_y, cell_count_z, cell_size_x, cell_size_y, cell_size_z),
	_behaviour_updaters{}, _prephysics_updaters{}, _physics_updaters{},
	_postphysics_updaters{}, _prerender_updaters{}, _render_updaters{},
	_update_interval{ 0.f }, _update_timeout{ 0.f }, _render_interval{ 0.f }, _render_timeout{ 0.f },
	_update_enabled{ true }, _render_enabled{ true }
{}


template <mv::uint dims>
void mv::Universe<dims>::update(float delta_time)
{
	if (!this->_update_enabled || (this->_update_timeout -= delta_time) >= 0.f)
		return;

	this->_update_timeout += this->_update_interval;
	this->_update_timeout = this->_update_timeout >= 0.f ? this->_update_timeout : 0.f;

	for (ComponentUpdaterBase<UpdateStage::behaviour>* updater : this->_behaviour_updaters) {
		updater->update(delta_time);
	}
	// update gridspace in parallel thread
	for (ComponentUpdaterBase<UpdateStage::prephysics>* updater : this->_prephysics_updaters) {
		updater->update(delta_time);
	}
	// wait for gridspace update
	for (ComponentUpdaterBase<UpdateStage::physics>* updater : this->_physics_updaters) {
		updater->update(delta_time);
	}
	// update gridspace in parallel thread
	for (ComponentUpdaterBase<UpdateStage::postphysics>* updater : this->_postphysics_updaters) {
		updater->update(delta_time);
	}
	// wait for gridspace update
}

template <mv::uint dims>
void mv::Universe<dims>::render(float delta_time)
{
	if (!this->_render_enabled)
		return;

	if ((this->_render_timeout -= this->_render_interval) < 0.f) {
		this->_render_timeout += this->_render_interval;
		this->_render_timeout = this->_render_timeout >= 0.f ? this->_render_timeout : 0.f;

		// calculate renderer model transform matrices in parallel thread
		for (ComponentUpdaterBase<UpdateStage::prerender>* updater : this->_prerender_updaters) {
			updater->update(delta_time);
		}
		// wait for model transform matrices to be calculated
		for (ComponentUpdaterBase<UpdateStage::render>* updater : this->_render_updaters) {
			updater->update(delta_time);
			updater->render();
		}
	}
	else {
		for (ComponentUpdaterBase<UpdateStage::render>* updater : this->_render_updaters) {
			updater->render();
		}
	}
}


template <mv::uint dims>
mv::Universe<dims>::Universe(Universe<dims>&& other) noexcept
	: _id{ other._id }, _gridspace{ std::move(other._gridspace) },
	_behaviour_updaters{ std::move(other._behaviour_updaters) }, _prephysics_updaters{ std::move(other._prephysics_updaters) },
	_physics_updaters{ std::move(other._physics_updaters) }, _postphysics_updaters{ std::move(other._postphysics_updaters) },
	_prerender_updaters{ std::move(other._prerender_updaters) }, _render_updaters{ std::move(other._render_updaters) },
	_update_interval{ other._update_interval }, _render_interval{ other._render_interval },
	_update_timeout{ other._update_timeout }, _render_timeout{ other._render_timeout },
	_update_enabled{ other._update_enabled }, _render_enabled{ other._render_enabled }
{
	other._id = invalid_id;
}


template <mv::uint dims>
mv::Universe<dims>& mv::Universe<dims>::operator=(Universe<dims>&& other) noexcept
{
	if (this == &other)
		return *this;
	this->_id = other._id;
	this->_gridspace = std::move(other._gridspace);
	this->_behaviour_updaters = std::move(other._behaviour_updaters);
	this->_prephysics_updaters = std::move(other._prephysics_updaters);
	this->_physics_updaters = std::move(other._physics_updaters);
	this->_postphysics_updaters = std::move(other._postphysics_updaters);
	this->_prerender_updaters = std::move(other._prerender_updaters);
	this->_render_updaters = std::move(other._render_updaters);
	this->_update_interval = other._update_interval;
	this->_render_interval = other._render_interval;
	this->_update_timeout = other._update_timeout;
	this->_render_timeout = other._render_timeout;
	this->_update_enabled = other._update_enabled;
	this->_render_enabled = other._render_enabled;
	other._id = invalid_id;
	return *this;
}


template <mv::uint dims>
mv::id_type mv::Universe<dims>::id() const
{
	return this->_id;
}


template <mv::uint dims>
mv::Entity<dims>& mv::Universe<dims>::spawn_entity(const transform_type& transform) const
{
	return mv::multiverse().create_entity<dims>(this->id());
}


template <mv::uint dims>
void mv::Universe<dims>::set_update_interval(float interval)
{
	this->_update_interval = interval >= 0.f ? interval : 0.f;
	this->_update_timeout = this->_update_interval < this->_update_timeout ? this->_update_interval : this->_update_timeout;
}

template <mv::uint dims>
void mv::Universe<dims>::set_update_enabled(bool enabled)
{
	this->_update_enabled = enabled;
}

template <mv::uint dims>
void mv::Universe<dims>::set_render_interval(float interval)
{
	this->_render_interval = interval >= 0.f ? interval : 0.f;
	this->_render_timeout = this->_render_interval < this->_render_timeout ? this->_render_interval : this->_render_timeout;
}

template <mv::uint dims>
void mv::Universe<dims>::set_render_enabled(bool enabled)
{
	this->_render_enabled = enabled;
}




template class mv::Universe<2>;
template mv::Universe<2>::Universe(id_type, uint, uint, float, float);
template class mv::Universe<2>::ComponentUpdaterList<mv::UpdateStage::behaviour>;
template class mv::Universe<2>::ComponentUpdaterList<mv::UpdateStage::prephysics>;
template class mv::Universe<2>::ComponentUpdaterList<mv::UpdateStage::physics>;
template class mv::Universe<2>::ComponentUpdaterList<mv::UpdateStage::postphysics>;
template class mv::Universe<2>::ComponentUpdaterList<mv::UpdateStage::prerender>;
template class mv::Universe<2>::ComponentUpdaterList<mv::UpdateStage::render>;
template mv::Universe<2>::Gridspace::Gridspace(uint, uint, float, float);
template void mv::Universe<2>::Gridspace::update_cells();
template std::vector<mv::Entity<2>*> mv::Universe<2>::Gridspace::entities_in_range(const position_type&, float) const;
template mv::uint mv::Universe<2>::Gridspace::_cell_count() const;
template class mv::Universe<3>;
template mv::Universe<3>::Universe(id_type, uint, uint, uint, float, float, float);
template class mv::Universe<3>::ComponentUpdaterList<mv::UpdateStage::behaviour>;
template class mv::Universe<3>::ComponentUpdaterList<mv::UpdateStage::prephysics>;
template class mv::Universe<3>::ComponentUpdaterList<mv::UpdateStage::physics>;
template class mv::Universe<3>::ComponentUpdaterList<mv::UpdateStage::postphysics>;
template class mv::Universe<3>::ComponentUpdaterList<mv::UpdateStage::prerender>;
template class mv::Universe<3>::ComponentUpdaterList<mv::UpdateStage::render>;
template mv::Universe<3>::Gridspace::Gridspace(uint, uint, uint, float, float, float);
template void mv::Universe<3>::Gridspace::update_cells();
template std::vector<mv::Entity<3>*> mv::Universe<3>::Gridspace::entities_in_range(const position_type&, float) const;
template mv::uint mv::Universe<3>::Gridspace::_cell_count() const;
