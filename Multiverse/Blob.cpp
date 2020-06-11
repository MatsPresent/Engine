#include "MultiversePCH.h"
#include "Blob.h"

#include <fstream>


mv::Blob::Blob(const std::string& path)
    : Resource()
{
	std::ifstream file(path, std::ios::binary);
    if (!file.is_open())
        throw std::runtime_error("failed to open binary file");

    file.seekg(0, file.end);
    this->_size = static_cast<size_type>(file.tellg());
    file.seekg(0, file.beg); 
    this->_data = new char[this->_size];

    file.read(this->_data, this->_size);
    file.close();
}

mv::Blob::Blob(Blob&& other) noexcept
    : Resource(std::move(other)), _size{ other._size }, _data{ other._data }
{
    other._size = 0;
    other._data = nullptr;
}


mv::Blob::~Blob()
{
    delete[] this->_data;
    this->_data = nullptr;
}


mv::Blob& mv::Blob::operator=(Blob&& other) noexcept
{
    if (this == &other) {
        return *this;
    }
    this->_size = other._size;
    this->_data = other._data;
    other._size = 0;
    other._data = nullptr;
    return *this;
}

mv::size_type mv::Blob::size() const
{
    return this->_size;
}

char mv::Blob::operator[](size_type i) const
{
    return this->_data[i];
}

const char* mv::Blob::data() const
{
    return this->_data;
}
