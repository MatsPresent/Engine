#include "MultiversePCH.h"
#include "Universe.h"

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
mv::Universe<dims>::Gridspace::Gridspace(mv::uint cell_count_x, mv::uint cell_count_y, float cell_size_x, float cell_size_y)
	: _cells{ new std::vector<id_type>[cell_count_x * cell_count_y] },
	_cell_counts{ cell_count_x, cell_count_y }, _cell_sizes{ cell_size_x, cell_size_y }
{}

template <mv::uint dims>
template <mv::uint _, typename std::enable_if<_ == 3, int>::type>
mv::Universe<dims>::Gridspace::Gridspace(mv::uint cell_count_x, mv::uint cell_count_y, mv::uint cell_count_z, float cell_size_x, float cell_size_y, float cell_size_z)
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
void mv::Universe<dims>::update(float deltaTime)
{
	for (ComponentUpdaterBase<UpdateStage::behaviour>* updater : this->_behaviour_updaters) {
		updater->update(deltaTime);
	}
	// update gridspace in parallel thread
	for (ComponentUpdaterBase<UpdateStage::prephysics>* updater : this->_prephysics_updaters) {
		updater->update(deltaTime);
	}
	// wait for gridspace update
	for (ComponentUpdaterBase<UpdateStage::physics>* updater : this->_physics_updaters) {
		updater->update(deltaTime);
	}
	// update gridspace in parallel thread
	for (ComponentUpdaterBase<UpdateStage::postphysics>* updater : this->_postphysics_updaters) {
		updater->update(deltaTime);
	}
	// wait for gridspace update
}

template <mv::uint dims>
void mv::Universe<dims>::render(float deltaTime)
{
	// calculate renderer model transform matrices in parallel thread
	for (ComponentUpdaterBase<UpdateStage::prerender>* updater : this->_prerender_updaters) {
		updater->update(deltaTime);
	}
	// wait for model transform matrices to be calculated
	for (ComponentUpdaterBase<UpdateStage::render>* updater : this->_render_updaters) {
		updater->update(deltaTime);
		updater->render();
	}
}

template class mv::Universe<2>;
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
