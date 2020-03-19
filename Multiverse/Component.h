#pragma once
#include "Entity.h"

namespace mv
{
	class Component
	{
		friend class Entity;


		id_type _id; // id of component, this is unique even across universes

	protected:
		Component() = default;

	public:
		id_type id() const;
	};
}
