#pragma once
struct SDL_Window;

namespace mv
{
	class Entity;
	class Universe;

	class Multiverse 
	{
	public:
		static const double tick_interval;
		static double frame_interval;

	private:
		static const unsigned int _tick_frequency;
		static SDL_Window* _window;
		static Entity* _entities;
		static Universe* _universes;


		Multiverse() = delete;

	public:
		static void init();
		static void cleanup();
		static void run();
	};
}