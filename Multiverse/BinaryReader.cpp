#include "MultiversePCH.h"
#include "BinaryReader.h"

mv::BinaryReader::BinaryReader(const Blob* blob)
	: _blob{ blob }, _head{ 0 }
{}

mv::BinaryReader& mv::BinaryReader::operator>>(std::string& target)
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
