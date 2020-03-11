#pragma once
struct SDL_Window;

namespace mv
{
	class Multiverse
	{
	public:
		void initialize();
		void load_game() const;
		void cleanup();
		void run();

	private:
		static const unsigned int _tick_frequency;
		static const float _tick_interval;
		SDL_Window* _window{};
	};
}