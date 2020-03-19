#include "MultiversePCH.h"
#include "Multiverse.h"

#include <chrono>
#include <type_traits>

#include "GameObject.h"
#include "InputManager.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "TextObject.h"


#ifndef MV_TICKFREQUENCY
#define MV_TICKFREQUENCY 60
#endif // !MG_TICKFREQUENCY
static_assert(std::is_integral<decltype(MV_TICKFREQUENCY)>::value && MV_TICKFREQUENCY > 0,
	"[mv] MV_TICKFREQUENCY must be a positive integral value");
static_assert(MV_TICKFREQUENCY <= 65536, "[mv] Tick frequency cannot exceed 65536 Hz");


const double mv::Multiverse::tick_interval = static_cast<double>(1'000'000'000 / MV_TICKFREQUENCY) / 1'000'000'000;
double mv::Multiverse::frame_interval = 0.;
const unsigned int mv::Multiverse::_tick_frequency = MV_TICKFREQUENCY;
SDL_Window* mv::Multiverse::_window = nullptr;


void mv::Multiverse::init()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}
	
	_window = SDL_CreateWindow("Programming 4 assignment", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		640, 480, SDL_WINDOW_OPENGL);
	if (_window == nullptr) {
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::instance().Init(_window);

	// tell the resource manager where he can find the game data
	ResourceManager::instance().Init("../Data/");
}

void mv::Multiverse::cleanup()
{
	Renderer::instance().Destroy();
	SDL_DestroyWindow(_window);
	_window = nullptr;
	SDL_Quit();
}

void mv::Multiverse::run()
{
	constexpr std::chrono::high_resolution_clock::duration tick_duration(
		std::chrono::duration_cast<std::chrono::high_resolution_clock::duration>(
		std::chrono::duration<long long, std::nano>(1'000'000'000 / _tick_frequency)));

	auto& scene_manager = SceneManager::instance();
	auto& input = InputManager::instance();
	auto& renderer = Renderer::instance();

	std::chrono::high_resolution_clock::time_point prev_time = std::chrono::high_resolution_clock::now();
	std::chrono::high_resolution_clock::duration behind_time(0);
	bool exit = false;
	while (!exit) {
		std::chrono::high_resolution_clock::time_point curr_time = std::chrono::high_resolution_clock::now();
		std::chrono::high_resolution_clock::duration elapsed_time = curr_time - prev_time; // add time since previous loop
		frame_interval = std::chrono::duration_cast<std::chrono::duration<double>>(elapsed_time).count();
		behind_time += elapsed_time;
		prev_time = curr_time;

		while (behind_time > tick_duration) {
			exit = !input.ProcessInput() || exit;
			scene_manager.Update();
			behind_time -= tick_duration;
		}

		renderer.Render();
	}
}
