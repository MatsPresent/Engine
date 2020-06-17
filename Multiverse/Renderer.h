#pragma once
#include "setup.h"

#include <string>

#include "Vector.h"

struct SDL_Window;

namespace mv
{
	class Multiverse;

	class Renderer final
	{
		friend Multiverse;

	public:
		struct Settings
		{
			struct {
				std::string title;
				uint16 width;
				uint16 height;
			} window;
			vec3f colour;
		};

	private:
		Settings _settings;
		SDL_Window* _window;
		void* _context;


		Renderer(const Settings& settings);
		Renderer(const Renderer&) = delete;
		Renderer(Renderer&& other) noexcept;

	public:
		~Renderer();

	private:
		Renderer& operator=(const Renderer&) = delete;
		Renderer& operator=(Renderer&& other) noexcept;

		void _init();

		void render() const;

	public:

	};
}

