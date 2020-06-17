#include "MultiversePCH.h"
#include "Renderer.h"

#include <SDL.h>
#include <glad/glad.h>

/*void mv::Renderer::Init(SDL_Window *)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	SDL_Window* window = SDL_CreateWindow("Programming 4 assignment", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		640, 480, SDL_WINDOW_OPENGL);
	if (window == nullptr) {
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GLContext context = SDL_GL_CreateContext(window);
	if (window == nullptr) {
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}
	//m_Renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (m_Renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}
}

void mv::Renderer::Render() const
{
	SDL_RenderClear(m_Renderer);

	SceneManager::instance().Render();
	
	SDL_RenderPresent(m_Renderer);
}

void mv::Renderer::Destroy()
{
	if (m_Renderer != nullptr)
	{
		SDL_DestroyRenderer(m_Renderer);
		m_Renderer = nullptr;
	}
}

void mv::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	SDL_Rect dst;
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void mv::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
{
	SDL_Rect dst;
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}*/


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
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	this->_window = SDL_CreateWindow(this->_settings.window.title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		this->_settings.window.width, this->_settings.window.height, SDL_WINDOW_OPENGL);
	if (this->_window == nullptr) {
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	this->_context = SDL_GL_CreateContext(this->_window);
	if (this->_context == nullptr) {
		throw std::runtime_error(std::string("SDL_GL_CreateContext Error: ") + SDL_GetError());
	} 
	
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		throw std::runtime_error(std::string("Failed to initialize GLAD"));
	}

	glClearColor(this->_settings.colour.x(), this->_settings.colour.y(), this->_settings.colour.z(), 1.f);
}


void mv::Renderer::render() const
{
	SDL_GL_MakeCurrent(this->_window, this->_context);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


}
