#include "BinaryReader.h"

template <typename T, typename std::enable_if<std::is_standard_layout<T>::value, int>::type>
inline mv::BinaryReader& mv::BinaryReader::operator>>(T& target)
{
	for (mv::size_type i = 0; i < static_cast<mv::size_type>(sizeof(T)); ++i) {
		reinterpret_cast<char*>(&target)[i] = (*this->_blob)[this->_head + i];
	}
	this->_head += sizeof(T);
	return *this;
}

template <typename T>
inline mv::BinaryReader& mv::BinaryReader::operator>>(std::vector<T>& target)
{
	mv::size_type size;
	this->operator>>(size);
	target.clear();
	target.reserve(size);
	for (mv::size_type i = 0; i < size; ++i) {
		this->operator>>(target[i]);
	}
	return *this;
}
