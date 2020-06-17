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
	using int8 = signed char;
	using int16 = short;
	using int32 = int;
	using int64 = long long;
	using uint8 = unsigned char;
	using uint16 = unsigned short;
	using uint32 = unsigned int;
	using uint64 = unsigned long long;

	using byte = uint8;
	using uint = uint32;

	using id_type = uint;
	using size_type = uint;
	using type_id_type = const void*;

	constexpr id_type invalid_id = static_cast<id_type>(-1);
	constexpr float pi = 3.14159265f;


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
