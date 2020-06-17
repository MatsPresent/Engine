#include "MultiversePCH.h"
#include "Texture.h"
#include "Renderer.h"

#include "glad/glad.h"
#include "stb_image.h"


mv::Texture::Texture(const std::string& path)
	: Resource(path), _texture_id{ 0 }, _width{}, _height{}
{}

mv::Texture::Texture(std::string&& path)
	: Resource(std::move(path)), _texture_id{ 0 }, _width{}, _height{}
{}


mv::size_type mv::Texture::width() const
{
	return this->_width;
}

mv::size_type mv::Texture::height() const
{
	return this->_height;
}


void mv::Texture::_load()
{
	static const int texture_formats[4]{ GL_RED, GL_RG, GL_RGB, GL_RGBA };
	int width, height, channel_count;
	byte* data = stbi_load(this->path().c_str(), &width, &height, &channel_count, 0);
	if (!data) {
		throw std::runtime_error(std::string("Failed to load texture"));
	}
	this->_width = width;
	this->_height = height;
	this->_channel_count = channel_count;
	int format = texture_formats[channel_count - 1];
	glGenTextures(1, &this->_texture_id);
	glBindTexture(GL_TEXTURE_2D, this->_texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);
}

void mv::Texture::_unload()
{
	glDeleteTextures(1, &this->_texture_id);
	this->_texture_id = 0;
}
