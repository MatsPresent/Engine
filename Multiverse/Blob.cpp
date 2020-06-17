#include "MultiversePCH.h"
#include "Blob.h"

#include <fstream>


mv::Blob::Blob(const std::string& path)
    : Resource(path), _size{ 0 }, _data{ nullptr }
{}

mv::Blob::Blob(std::string&& path)
    : Resource(std::move(path)), _size{ 0 }, _data{ nullptr }
{}


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


void mv::Blob::_load()
{
    std::ifstream file(this->path(), std::ios::binary);
    if (!file.is_open())
        throw std::runtime_error("failed to open binary file");

    file.seekg(0, file.end);
    this->_size = static_cast<size_type>(file.tellg());
    file.seekg(0, file.beg);
    this->_data = new char[this->_size];

    file.read(this->_data, this->_size);
    file.close();
}

void mv::Blob::_unload()
{
    delete[] this->_data;
    this->_data = nullptr;
}
