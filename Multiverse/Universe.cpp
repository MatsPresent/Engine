#include "MultiversePCH.h"
#include "Universe.h"

#include <algorithm> // find
#include <cmath>

#include "Entity.h"
#include "Multiverse.h"
#include "Component.h"
#include "ThreadPool.h"

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
	: _cells{ new Cell[cell_count_x * cell_count_y]{} },
	_cell_counts{ cell_count_x, cell_count_y }, _cell_sizes{ cell_size_x, cell_size_y }
{}

template <mv::uint dims>
template <mv::uint _, typename std::enable_if<_ == 3, int>::type>
mv::Universe<dims>::Gridspace::Gridspace(
	mv::uint cell_count_x, mv::uint cell_count_y, mv::uint cell_count_z, float cell_size_x, float cell_size_y, float cell_size_z)
	: _cells{ new Cell[cell_count_x * cell_count_y * cell_count_z]{} },
	_cell_counts{ cell_count_x, cell_count_y, cell_count_z }, _cell_sizes{ cell_size_x, cell_size_y, cell_size_z }
{}

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
void mv::Universe<dims>::Gridspace::add(id_type entity_id)
{
	Entity<dims>& e = mv::multiverse().entity<dims>(entity_id);
	uint cell = this->_calculate_cell(e.get_transform().translate);
	e._gridspace_cell_idx = cell;
	if (e.is_static()) {
		this->_cells[cell].static_entity_ids.push_back(entity_id);
	}
	else {
		this->_cells[cell].dynamic_entity_ids.push_back(entity_id);
		++this->_cells[cell].count;
	}
}

template <mv::uint dims>
void mv::Universe<dims>::Gridspace::remove(id_type entity_id)
{
	Entity<dims>& e = mv::multiverse().entity<dims>(entity_id);
	uint cell = e._gridspace_cell_idx;
	std::vector<id_type>& vec = e.is_static() ? this->_cells[cell].static_entity_ids : this->_cells[cell].dynamic_entity_ids;
	auto it = std::find(vec.begin(), vec.end(), entity_id);
	*it = vec.back();
	vec.pop_back();
	if (!e.is_static()) {
		--this->_cells[cell].count;
	}
}


template <mv::uint dims>
template <mv::uint _, typename std::enable_if<_ == 2, int>::type>
void mv::Universe<dims>::Gridspace::update_cells()
{
	for (uint i = 0; i < this->_cell_count(); ++i) {
		for (uint j = 0; j < this->_cells[i].count; ++j) {
			Entity<2>& e = mv::multiverse().entity<2>(this->_cells[i].dynamic_entity_ids[j]);
			uint new_cell = this->_calculate_cell(e._transform.translate);
			e._gridspace_cell_idx = new_cell;
			if (new_cell != i) {
				this->_cells[new_cell].dynamic_entity_ids.push_back(this->_cells[i].dynamic_entity_ids[j]);
				this->_cells[i].dynamic_entity_ids[j] = this->_cells[i].dynamic_entity_ids.back();
				this->_cells[i].dynamic_entity_ids.pop_back();
				--this->_cells[i].count;
				--j;
				if (new_cell < i) {
					++this->_cells[new_cell].count;
				}
			}
			e._transform_buffer = e._transform;
		}
		this->_cells[i].count = static_cast<uint>(this->_cells[i].dynamic_entity_ids.size());
	}
}

template <mv::uint dims>
template <mv::uint _, typename std::enable_if<_ == 3, int>::type>
void mv::Universe<dims>::Gridspace::update_cells()
{

}


template <mv::uint dims>
template <mv::uint _, typename std::enable_if<_ == 2, int>::type>
void mv::Universe<dims>::Gridspace::update_collision() const
{
	float radius = this->_cell_sizes[0]; // bad value for scan radius, obviously will miss some objects larger than a cell
	float sqr_radius = radius * radius;
	for (uint i = 0; i < this->_cell_count(); ++i) {
		for (uint j = 0; j < this->_cells[i].count; ++j) {
			Entity<2>& a = mv::multiverse().entity<2>(this->_cells[i].dynamic_entity_ids[j]);
			auto origin = a._transform.translate;
			uint xmin, xmax, ymin, ymax;
			if (radius * 2.f < this->_cell_sizes[0] * static_cast<float>(this->_cell_counts[0] - 1)) {
				xmin = this->_calculate_grid_coord(origin.x() - radius, 0);
				xmax = this->_calculate_grid_coord(origin.x() + radius, 0) + 1;
			}
			else {
				xmin = 0;
				xmax = this->_cell_counts[0];
			}
			if (radius * 2.f < this->_cell_sizes[1] * static_cast<float>(this->_cell_counts[1] - 1)) {
				ymin = this->_calculate_grid_coord(origin.y() - radius, 1);
				ymax = this->_calculate_grid_coord(origin.y() + radius, 1) + 1;
			}
			else {
				ymin = 0;
				ymax = this->_cell_counts[1];
			}

			for (uint y = ymin; y != ymax; y = (y + 1) % this->_cell_counts[1]) {
				for (uint x = xmin; x != xmax; x = (x + 1) % this->_cell_counts[0]) {
					for (id_type entity_id : this->_cells[x + this->_cell_counts[0] * y].static_entity_ids) {
						Entity<2>& b = mv::multiverse().entity<2>(entity_id);
						if ((b.get_transform().translate - origin).squared_magnitude() < sqr_radius) {
							a._solve_collision(b);
						}
					}
					for (id_type entity_id : this->_cells[x + this->_cell_counts[0] * y].dynamic_entity_ids) {
						Entity<2>& b = mv::multiverse().entity<2>(entity_id);
						if (entity_id < b.id()) {
							continue; // only check each pair once
						}
						if ((b.get_transform().translate - origin).squared_magnitude() < sqr_radius) {
							a._solve_collision(b);
						}
					}
				}
			}
		}
	}	
}

template <mv::uint dims>
template <mv::uint _, typename std::enable_if<_ == 3, int>::type>
void mv::Universe<dims>::Gridspace::update_collision() const
{}


template <mv::uint dims>
template <mv::uint _, typename std::enable_if<_ == 2, int>::type>
std::vector<mv::Entity<2>*> mv::Universe<dims>::Gridspace::entities_in_range(const position_type& origin, float radius) const
{
	static std::vector<mv::Entity<2>*> retval;

	uint xmin, xmax, ymin, ymax;
	if (radius * 2.f < this->_cell_sizes[0] * static_cast<float>(this->_cell_counts[0] - 1)) {
		xmin = this->_calculate_grid_coord(origin.x() - radius, 0);
		xmax = this->_calculate_grid_coord(origin.x() + radius, 0) + 1;
	}
	else {
		xmin = 0;
		xmax = this->_cell_counts[0];
	}
	if (radius * 2.f < this->_cell_sizes[1] * static_cast<float>(this->_cell_counts[1] - 1)) {
		ymin = this->_calculate_grid_coord(origin.y() - radius, 1);
		ymax = this->_calculate_grid_coord(origin.y() + radius, 1) + 1;
	}
	else {
		ymin = 0;
		ymax = this->_cell_counts[1];
	}

	float sqr_radius = radius * radius;
	for (uint y = ymin; y != ymax; y = (y + 1) % this->_cell_counts[1]) {
		for (uint x = xmin; x != xmax; x = (x + 1) % this->_cell_counts[0]) {
			for (id_type entity_id : this->_cells[x + this->_cell_counts[0] * y].static_entity_ids) {
				Entity<2>& e = mv::multiverse().entity<2>(entity_id);
				if ((e.get_transform().translate - origin).squared_magnitude() < sqr_radius) {
					retval.push_back(&e);
				}
			}
			for (id_type entity_id : this->_cells[x + this->_cell_counts[0] * y].dynamic_entity_ids) {
				Entity<2>& e = mv::multiverse().entity<2>(entity_id);
				if ((e.get_transform().translate - origin).squared_magnitude() < sqr_radius) {
					retval.push_back(&e);
				}
			}
		}
	}
	return retval;
}

template <mv::uint dims>
template <mv::uint _, typename std::enable_if<_ == 3, int>::type>
std::vector<mv::Entity<3>*> mv::Universe<dims>::Gridspace::entities_in_range(const position_type&, float) const
{
	static std::vector<mv::Entity<3>*> retval;

	return retval;
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
inline mv::uint mv::Universe<dims>::Gridspace::_calculate_cell(const position_type& position) const
{
	uint x = this->_calculate_grid_coord(position.x(), 0);
	uint y = this->_calculate_grid_coord(position.y(), 1);
	return x + this->_cell_counts[0] * y;
}

template <mv::uint dims>
template <mv::uint _, typename std::enable_if<_ == 3, int>::type>
inline mv::uint mv::Universe<dims>::Gridspace::_calculate_cell(const position_type&) const
{
	return 0;
}

template <mv::uint dims>
inline mv::uint mv::Universe<dims>::Gridspace::_calculate_grid_coord(float world_coord, uint coord_idx) const
{
	auto mod = [](float n, uint d) {
		return static_cast<uint>(n - std::floor(n / static_cast<float>(d))* static_cast<float>(d));
	};
	return mod(world_coord / this->_cell_sizes[coord_idx], this->_cell_counts[coord_idx]);
}




template <mv::uint dims>
template <mv::uint _, typename std::enable_if<_ == 2, int>::type>
mv::Universe<dims>::Universe(
	id_type id, uint cell_count_x, uint cell_count_y, float cell_size_x, float cell_size_y)
	: _id{ id }, _gridspace(cell_count_x, cell_count_y, cell_size_x, cell_size_y),
	_physics_updaters{}, _postphysics_updaters{}, _input_updaters{},
	_behaviour_updaters{}, _prerender_updaters{}, _render_updaters{},
	_update_interval{ 0.f }, _update_timeout{ 0.f }, _render_interval{ 0.f }, _render_timeout{ 0.f },
	_update_enabled{ true }, _render_enabled{ true },
	_transform_readonly{ false }, _transform_read_buffer{ false }
{}

template <mv::uint dims>
template <mv::uint _, typename std::enable_if<_ == 3, int>::type>
mv::Universe<dims>::Universe(
	id_type id, uint cell_count_x, uint cell_count_y, uint cell_count_z, float cell_size_x, float cell_size_y, float cell_size_z)
	: _id{ id }, _gridspace(cell_count_x, cell_count_y, cell_count_z, cell_size_x, cell_size_y, cell_size_z),
	_physics_updaters{}, _postphysics_updaters{}, _input_updaters{},
	_behaviour_updaters{}, _prerender_updaters{}, _render_updaters{},
	_update_interval{ 0.f }, _update_timeout{ 0.f }, _render_interval{ 0.f }, _render_timeout{ 0.f },
	_update_enabled{ true }, _render_enabled{ true },
	_transform_readonly{ false }, _transform_read_buffer{ false }
{}


template <mv::uint dims>
void mv::Universe<dims>::add_entity(id_type entity_id)
{
	this->_gridspace.add(entity_id);
}

template <mv::uint dims>
void mv::Universe<dims>::remove_entity(id_type entity_id)
{
	this->_gridspace.remove(entity_id);
}

template <mv::uint dims>
void mv::Universe<dims>::remove_component(UpdateStage stage, type_id_type component_type_id, id_type component_id)
{
	switch (stage)
	{
	case mv::UpdateStage::input:
		this->_input_updaters.remove(component_type_id, component_id);
		break;
	case mv::UpdateStage::behaviour:
		this->_behaviour_updaters.remove(component_type_id, component_id);
		break;
	case mv::UpdateStage::physics:
		this->_physics_updaters.remove(component_type_id, component_id);
		break;
	case mv::UpdateStage::postphysics:
		this->_postphysics_updaters.remove(component_type_id, component_id);
		break;
	case mv::UpdateStage::prerender:
		this->_prerender_updaters.remove(component_type_id, component_id);
		break;
	case mv::UpdateStage::render:
		this->_render_updaters.remove(component_type_id, component_id);
		break;
	}
}


template <mv::uint dims>
void mv::Universe<dims>::update(float delta_time)
{
	if (!this->_update_enabled || (this->_update_timeout -= delta_time) >= 0.f)
		return;

	this->_update_timeout += this->_update_interval;
	this->_update_timeout = this->_update_timeout >= 0.f ? this->_update_timeout : 0.f;


	this->_transform_read_buffer = false;
	for (ComponentUpdaterBase<UpdateStage::physics>* updater : this->_physics_updaters) {
		updater->update(delta_time);
	}

	this->_transform_readonly = true;
	std::future<void> gridspace_update_result = multiverse().thread_pool().enqueue(&Gridspace::template update_cells<dims>, std::ref(this->_gridspace));
	for (ComponentUpdaterBase<UpdateStage::postphysics>* updater : this->_postphysics_updaters) {
		updater->update(delta_time);
	}
	gridspace_update_result.get();
	this->_transform_read_buffer = true;
	std::future<void> collision_update_result = multiverse().thread_pool().enqueue(&Gridspace::template update_collision<dims>, std::cref(this->_gridspace));
	for (ComponentUpdaterBase<UpdateStage::input>* updater : this->_input_updaters) {
		updater->update(delta_time);
	}
	collision_update_result.get();
	this->_transform_readonly = false;

	for (ComponentUpdaterBase<UpdateStage::behaviour>* updater : this->_behaviour_updaters) {
		updater->update(delta_time);
	}
}

template <mv::uint dims>
void mv::Universe<dims>::render(float delta_time)
{
	if (!this->_render_enabled)
		return;

	if ((this->_render_timeout -= this->_render_interval) < 0.f) {
		this->_render_timeout += this->_render_interval;
		this->_render_timeout = this->_render_timeout >= 0.f ? this->_render_timeout : 0.f;

		this->_transform_readonly = true;
		// calculate renderer model transform matrices in parallel thread
		for (ComponentUpdaterBase<UpdateStage::prerender>* updater : this->_prerender_updaters) {
			updater->update(delta_time);
		}
		// wait for model transform matrices to be calculated
		this->_transform_readonly = false;
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
	_physics_updaters{ std::move(other._physics_updaters) }, _postphysics_updaters{ std::move(other._postphysics_updaters) },
	_input_updaters{ std::move(other._input_updaters) }, _behaviour_updaters{ std::move(other._behaviour_updaters) },
	_prerender_updaters{ std::move(other._prerender_updaters) }, _render_updaters{ std::move(other._render_updaters) },
	_update_interval{ other._update_interval }, _render_interval{ other._render_interval },
	_update_timeout{ other._update_timeout }, _render_timeout{ other._render_timeout },
	_update_enabled{ other._update_enabled }, _render_enabled{ other._render_enabled },
	_transform_readonly{ other._transform_readonly }, _transform_read_buffer{ other._transform_read_buffer }
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
	this->_physics_updaters = std::move(other._physics_updaters);
	this->_postphysics_updaters = std::move(other._postphysics_updaters);
	this->_input_updaters = std::move(other._input_updaters);
	this->_behaviour_updaters = std::move(other._behaviour_updaters);
	this->_prerender_updaters = std::move(other._prerender_updaters);
	this->_render_updaters = std::move(other._render_updaters);
	this->_update_interval = other._update_interval;
	this->_render_interval = other._render_interval;
	this->_update_timeout = other._update_timeout;
	this->_render_timeout = other._render_timeout;
	this->_update_enabled = other._update_enabled;
	this->_render_enabled = other._render_enabled;
	this->_transform_readonly = other._transform_readonly;
	this->_transform_read_buffer = other._transform_read_buffer;
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
	return mv::multiverse().create_entity<dims>(this->id(), transform);
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
template class mv::Universe<2>::ComponentUpdaterList<mv::UpdateStage::input>;
template class mv::Universe<2>::ComponentUpdaterList<mv::UpdateStage::behaviour>;
template class mv::Universe<2>::ComponentUpdaterList<mv::UpdateStage::physics>;
template class mv::Universe<2>::ComponentUpdaterList<mv::UpdateStage::postphysics>;
template class mv::Universe<2>::ComponentUpdaterList<mv::UpdateStage::prerender>;
template class mv::Universe<2>::ComponentUpdaterList<mv::UpdateStage::render>;
template mv::Universe<2>::Gridspace::Gridspace(uint, uint, float, float);
template void mv::Universe<2>::Gridspace::update_cells();
template std::vector<mv::Entity<2>*> mv::Universe<2>::Gridspace::entities_in_range(const position_type&, float) const;
template mv::uint mv::Universe<2>::Gridspace::_cell_count() const;
template mv::uint mv::Universe<2>::Gridspace::_calculate_cell(const position_type&) const;
template class mv::Universe<3>;
template mv::Universe<3>::Universe(id_type, uint, uint, uint, float, float, float);
template class mv::Universe<3>::ComponentUpdaterList<mv::UpdateStage::input>;
template class mv::Universe<3>::ComponentUpdaterList<mv::UpdateStage::behaviour>;
template class mv::Universe<3>::ComponentUpdaterList<mv::UpdateStage::physics>;
template class mv::Universe<3>::ComponentUpdaterList<mv::UpdateStage::postphysics>;
template class mv::Universe<3>::ComponentUpdaterList<mv::UpdateStage::prerender>;
template class mv::Universe<3>::ComponentUpdaterList<mv::UpdateStage::render>;
template mv::Universe<3>::Gridspace::Gridspace(uint, uint, uint, float, float, float);
template void mv::Universe<3>::Gridspace::update_cells();
template std::vector<mv::Entity<3>*> mv::Universe<3>::Gridspace::entities_in_range(const position_type&, float) const;
template mv::uint mv::Universe<3>::Gridspace::_cell_count() const;
template mv::uint mv::Universe<3>::Gridspace::_calculate_cell(const position_type&) const;
