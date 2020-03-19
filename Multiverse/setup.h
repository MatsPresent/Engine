#pragma once

namespace mv
{
	using id_type = unsigned int;


	template <typename T>
	constexpr auto type_id() -> const void*;

	template <typename T>
	class TypeID
	{
		static constexpr const T* const _id{ nullptr };

		TypeID() = delete;

	public:
		friend constexpr auto type_id<T>() -> const void*;
	};

	template <typename T>
	constexpr auto type_id() -> const void*
	{
		return &TypeID<T>::_id;
	}
}
