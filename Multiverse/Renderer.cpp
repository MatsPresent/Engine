#include "MultiversePCH.h"
#include "Renderer.h"

#include <SDL.h>
#include <glad/glad.h>

#include "Debug.h"


mv::Renderer::Renderer(const Settings& settings)
	: _settings(settings), _window{ nullptr }, _context{ nullptr }
{
	this->_init();
}

mv::Renderer::Renderer(Renderer&& other) noexcept
	: _settings(other._settings), _window{ other._window }, _context{ other._context }
{
	other._window = nullptr;
	other._context = nullptr;
}


mv::Renderer::~Renderer()
{
	SDL_DestroyWindow(this->_window);
	SDL_GL_DeleteContext(this->_context);
	SDL_Quit();
}


mv::Renderer& mv::Renderer::operator=(Renderer&& other) noexcept
{
	if (this == &other)
		return *this;
	this->_window = other._window;
	this->_context = other._context;
	other._window = nullptr;
	other._context = nullptr;
	return *this;
}


void mv::Renderer::_init()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		debug->error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	this->_window = SDL_CreateWindow(this->_settings.window.title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		this->_settings.window.width, this->_settings.window.height, SDL_WINDOW_OPENGL);
	if (this->_window == nullptr) {
		debug->error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	this->_context = SDL_GL_CreateContext(this->_window);
	if (this->_context == nullptr) {
		debug->error(std::string("SDL_GL_CreateContext Error: ") + SDL_GetError());
	} 
	
	if (!gladLoadGLLoader(&SDL_GL_GetProcAddress)) {
		debug->error(std::string("Failed to initialize GLAD"));
	}

	glClearColor(this->_settings.colour.x(), this->_settings.colour.y(), this->_settings.colour.z(), 1.f);
	glViewport(0, 0, this->_settings.window.width, this->_settings.window.height);
}


void mv::Renderer::render() const
{
	SDL_GL_MakeCurrent(this->_window, this->_context);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	SDL_GL_SwapWindow(this->_window);
}
