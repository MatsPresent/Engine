#include "MultiversePCH.h"
#include "ResourceManager.h"

#include <filesystem>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Multiverse.h"
#include "Renderer.h"
#include "Texture.h"
#include "Blob.h"
#include "SpriteSheet.h"


mv::ResourceManager::ResourceManager(const std::string& data_path)
	: _data_path{ std::filesystem::path(data_path).generic_string() }, _resources()
{
	this->_init();
}

mv::ResourceManager::ResourceManager(ResourceManager&& other) noexcept
	: _data_path(std::move(other._data_path)), _resources(std::move(other._resources))
{}


mv::ResourceManager::~ResourceManager()
{
	for (auto& e : this->_resources) {
		e.second->unload();
	}
	for (auto& e : this->_resources) {
		delete e.second;
	}
	this->_resources.clear();
}


mv::ResourceManager& mv::ResourceManager::operator=(ResourceManager&& other) noexcept
{
	if (this == &other)
		return *this;
	this->_data_path = std::move(other._data_path);
	this->_resources = std::move(other._resources);
	return *this;
}


void mv::ResourceManager::_init()
{
	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
		throw std::runtime_error(std::string("Failed to load support for png's: ") + SDL_GetError());
	}
	if ((IMG_Init(IMG_INIT_JPG) & IMG_INIT_JPG) != IMG_INIT_JPG) {
		throw std::runtime_error(std::string("Failed to load support for jpg's: ") + SDL_GetError());
	}
	if (TTF_Init() != 0) {
		throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
	}

	for (const std::filesystem::directory_entry& e : std::filesystem::recursive_directory_iterator(this->_data_path)) {
		if (e.is_regular_file()) {
			this->_register_resource(e.path().generic_string(), e.path().extension().generic_string());
		}
	}

	for (auto& e : this->_resources) {
		e.second->load();
	}
}

void mv::ResourceManager::_register_resource(const std::string& path, const std::string& extension)
{
	std::string id = path.substr(this->_data_path.size());
	if (extension == ".jpg" || extension == ".png") {
		this->_resources.emplace(id, new Texture(path));
	}
	else if (extension == ".otf" || extension == ".ttf") {
		// load font
	}
	else if (extension == ".mvs") {
		this->_resources.emplace(id, new SpriteSheet(path));
	}
	else {
		this->_resources.emplace(id, new Blob(path));
	}
}


template <typename T>
const T* mv::ResourceManager::get(const std::string& id) const
{
	auto it = this->_resources.find(id);
	if (it == this->_resources.end()) {
		return nullptr;
	}
	return dynamic_cast<const T*>(it->second);
}

template const mv::Texture* mv::ResourceManager::get<mv::Texture>(const std::string&) const;
template const mv::SpriteSheet* mv::ResourceManager::get<mv::SpriteSheet>(const std::string&) const;
template const mv::Blob* mv::ResourceManager::get<mv::Blob>(const std::string&) const;
