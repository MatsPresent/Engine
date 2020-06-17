#include "MultiversePCH.h"
#include "Resource.h"

#include "ResourceManager.h"


mv::Resource::Resource(const std::string& path)
	: _path{ path }, _loaded{ false }
{}

mv::Resource::Resource(std::string&& path)
	: _path{ std::move(path) }, _loaded{ false }
{}


mv::Resource::~Resource()
{
	if (this->loaded()) {
		std::cerr << "Called destructor on loaded resource: " << this->path() << '\n';
	}
}


const std::string& mv::Resource::path() const
{
	return this->_path;
}


bool mv::Resource::loaded() const
{
	return this->_loaded;
}


void mv::Resource::load()
{
	if (this->_loaded) {
		return;
	}
	this->_load();
	this->_loaded = true;
}

void mv::Resource::unload()
{
	if (!this->_loaded) {
		return;
	}
	this->_loaded = false;
	this->_unload();
}
