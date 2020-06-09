#pragma once
#include <cstddef>

#ifndef MV_CELL_COUNT_DEFAULT
#define MV_CELL_COUNT_DEFAULT 16
#endif
#ifndef MV_CELL_SIZE_DEFAULT
#define MV_CELL_SIZE_DEFAULT 16.f
#endif

namespace mv
{
	using uint = unsigned int;
	using id_type = uint;
	using size_type = uint;
	using type_id_type = const void*;

	constexpr id_type invalid_id = static_cast<id_type>(-1);


	template <typename T>
	class TypeID
	{	
		static constexpr const T* const _id{ nullptr };

	public:
		static constexpr type_id_type value = static_cast<type_id_type>(&_id);

		TypeID() = delete;
	};

	template <typename T>
	constexpr type_id_type type_id()
	{
		return TypeID<T>::value;
	}

	class Multiverse;
	Multiverse& multiverse();
}
