#include "MiniginPCH.h"
#include "Multiverse.h"

#include <chrono>
#include <thread>

#include <SDL.h>
#include "GameObject.h"
#include "InputManager.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "TextObject.h"

using namespace std;
using namespace std::chrono;


#ifndef MG_TICKFREQUENCY
#define MG_TICKFREQUENCY 60
#endif // !MG_TICKFREQUENCY


const unsigned int mv::Multiverse::_tick_frequency = MG_TICKFREQUENCY;
const float mv::Multiverse::_tick_interval = 10.f / static_cast<float>(MG_TICKFREQUENCY);


void mv::Multiverse::initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	_window = SDL_CreateWindow("Programming 4 assignment", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		640, 480, SDL_WINDOW_OPENGL );
	if (_window == nullptr) {
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::instance().Init(_window);
}

/**
 * Code constructing the scene world starts here
 */
void mv::Multiverse::load_game() const
{
	auto& scene = SceneManager::instance().CreateScene("Demo");

	auto go = std::make_shared<GameObject>();
	go->SetTexture("background.jpg");
	scene.Add(go);

	go = std::make_shared<GameObject>();
	go->SetTexture("logo.png");
	go->SetPosition(216, 180);
	scene.Add(go);

	auto font = ResourceManager::instance().LoadFont("Lingua.otf", 36);
	auto to = std::make_shared<TextObject>("Programming 4 Assignment", font);
	to->SetPosition(80, 20);
	scene.Add(to);
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
	initialize();

	// tell the resource manager where he can find the game data
	ResourceManager::instance().Init("../Data/");

	load_game();

	auto& scene_manager = SceneManager::instance();
	auto& input = InputManager::instance();
	auto& renderer = Renderer::instance();

	const high_resolution_clock::duration tick_duration(1'000'000'000 / _tick_frequency);
	high_resolution_clock::time_point prev_time = high_resolution_clock::now();
	high_resolution_clock::duration behind_time;
	bool exit = false;
	while (!exit) {
		high_resolution_clock::time_point curr_time = high_resolution_clock::now();
		high_resolution_clock::duration delta_time = curr_time - prev_time; // time since previous loop
		prev_time = curr_time;
		behind_time += delta_time;
			
		exit = !input.ProcessInput();

		while (behind_time > tick_duration) {
			scene_manager.Update();
			behind_time -= tick_duration;
		}

		renderer.Render();
	}

	cleanup();
}
