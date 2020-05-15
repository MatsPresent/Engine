#include "MultiversePCH.h"
#include "Entity.h"

template <unsigned int dims>
inline mv::id_type mv::Entity<dims>::id() const
{
	return this->_id;
}

template <unsigned int dims>
mv::id_type mv::Entity<dims>::universe_id() const
{
	return this->_universe_id;
}

template <unsigned int dims>
const mv::Universe<dims>& mv::Entity<dims>::universe() const
{
	return mv::multiverse()._universe<dims>(this->_universe_id);
}

template <unsigned int dims>
mv::Universe<dims>& mv::Entity<dims>::_universe()
{
	return mv::multiverse()._universe<dims>(this->_universe_id);
}

template class mv::Entity<2>;
template class mv::Entity<3>;
