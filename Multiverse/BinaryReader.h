#pragma once
#include <type_traits>
#include <string>
#include <vector>
#include "Blob.h"

namespace mv
{
	class BinaryReader
	{
	private:
		const Blob* _blob;
		mv::size_type _head;
		
	public:
		BinaryReader(const Blob* blob);

		template <typename T, typename std::enable_if<std::is_standard_layout<T>::value, int>::type = 0>
		BinaryReader& operator>>(T& target);
		BinaryReader& operator>>(std::string& target);
		template <typename T>
		BinaryReader& operator>>(std::vector<T>& target);
	};
}

#include "BinaryReader.inl"
