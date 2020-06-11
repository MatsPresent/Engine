#include "MultiversePCH.h"
#include "Multiverse.h"

#include <chrono>
#include <type_traits>

#include "Entity.h"
#include "Universe.h"

#include "GameObject.h"
#include "InputManager.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "TextObject.h"
#include "ThreadPool.h"
#include "Transform.h"


#ifndef MV_TICKFREQUENCY
#define MV_TICKFREQUENCY 60
#endif // !MG_TICKFREQUENCY
static_assert(std::is_integral<decltype(MV_TICKFREQUENCY)>::value && MV_TICKFREQUENCY > 0,
	"[mv] MV_TICKFREQUENCY must be a positive integral value");
static_assert(MV_TICKFREQUENCY <= 65536, "[mv] Tick frequency cannot exceed 65536 Hz");


const float mv::Multiverse::tick_interval = static_cast<float>(1'000'000'000 / MV_TICKFREQUENCY) / 1'000'000'000;
const mv::uint mv::Multiverse::tick_frequency = MV_TICKFREQUENCY;


mv::Multiverse& mv::multiverse()
{
	return Multiverse::get();
}


mv::Multiverse::Multiverse() = default;

mv::Multiverse::~Multiverse() = default;

mv::Multiverse& mv::Multiverse::get()
{
	static Multiverse instance;
	return instance;
}

void mv::Multiverse::init()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}
	
	this->_window = SDL_CreateWindow("Programming 4 assignment", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		640, 480, SDL_WINDOW_OPENGL);
	if (this->_window == nullptr) {
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::instance().Init(this->_window);

	this->_resource_manager = new ResourceManager("../Data/");
	this->_thread_pool = new ThreadPool(std::min(1, static_cast<int>(std::thread::hardware_concurrency()) - 1));
}

void mv::Multiverse::cleanup()
{
	Renderer::instance().Destroy();
	delete this->_resource_manager;
	SDL_DestroyWindow(this->_window);
	this->_window = nullptr;
	SDL_Quit();
}

void mv::Multiverse::run()
{
	constexpr std::chrono::high_resolution_clock::duration tick_duration(
		std::chrono::duration_cast<std::chrono::high_resolution_clock::duration>(
		std::chrono::duration<long long, std::nano>(1'000'000'000 / tick_frequency)));

	auto& input = InputManager::instance();
	auto& renderer = Renderer::instance();

	std::chrono::high_resolution_clock::time_point prev_time = std::chrono::high_resolution_clock::now();
	std::chrono::high_resolution_clock::duration behind_time(0);
	bool exit = false;
	while (!exit) {
		std::chrono::high_resolution_clock::time_point curr_time = std::chrono::high_resolution_clock::now();
		std::chrono::high_resolution_clock::duration elapsed_time = curr_time - prev_time; // add time since previous loop
		float frame_interval = std::chrono::duration_cast<std::chrono::duration<float>>(elapsed_time).count();
		behind_time += elapsed_time;
		prev_time = curr_time;

		while (behind_time > tick_duration) {
			exit = !input.ProcessInput() || exit;
			for (Universe<2>& universe : this->_universes2d) {
				universe.update(this->tick_interval);
			}
			for (Universe<3>& universe : this->_universes3d) {
				universe.update(this->tick_interval);
			}
			behind_time -= tick_duration;
		}

		SDL_RenderClear(renderer.GetSDLRenderer());
		for (Universe<2>& universe : this->_universes2d) {
			universe.render(frame_interval);
		}
		for (Universe<3>& universe : this->_universes3d) {
			universe.render(frame_interval);
		}
		SDL_RenderPresent(renderer.GetSDLRenderer());
	}
}


mv::ResourceManager& mv::Multiverse::resource_manager() const
{
	return *this->_resource_manager;
}

mv::ThreadPool& mv::Multiverse::thread_pool() const
{
	return *this->_thread_pool;
}


template <mv::uint dims, typename std::enable_if<dims == 2, int>::type>
inline mv::Entity<2>& mv::Multiverse::entity(id_type id)
{
	return this->_entities2d[id];
}

template <mv::uint dims, typename std::enable_if<dims == 3, int>::type>
inline mv::Entity<3>& mv::Multiverse::entity(id_type id)
{
	return this->_entities3d[id];
}


template <mv::uint dims, typename std::enable_if<dims == 2, int>::type>
mv::Universe<2>& mv::Multiverse::universe(id_type id)
{
	return this->_universes2d[id];
}

template <mv::uint dims, typename std::enable_if<dims == 3, int>::type>
mv::Universe<3>& mv::Multiverse::universe(id_type id)
{
	return this->_universes3d[id];
}


template <mv::uint dims, typename std::enable_if<dims == 2, int>::type>
mv::Entity<2>& mv::Multiverse::create_entity(id_type universe_id)
{
	id_type id = this->_entities2d.insert(Entity<2>{ this->_entities2d.next_id(), universe_id, Transform<2>{} });
	this->_universes2d[universe_id].add_entity(id);
	return this->_entities2d[id];
}

template <mv::uint dims, typename std::enable_if<dims == 2, int>::type>
mv::Entity<2>& mv::Multiverse::create_entity(id_type universe_id, const Transform<2>& transform)
{
	id_type id = this->_entities2d.insert(Entity<2>{ this->_entities2d.next_id(), universe_id, transform });
	this->_universes2d[universe_id].add_entity(id);
	return this->_entities2d[id];
}

template <mv::uint dims, typename std::enable_if<dims == 3, int>::type>
mv::Entity<3>& mv::Multiverse::create_entity(id_type universe_id)
{
	id_type id = this->_entities3d.insert(Entity<3>{ this->_entities3d.next_id(), universe_id, Transform<3>{} });
	this->_universes3d[universe_id].add_entity(id);
	return this->_entities3d[id];
}

template <mv::uint dims, typename std::enable_if<dims == 3, int>::type>
mv::Entity<3>& mv::Multiverse::create_entity(id_type universe_id, const Transform<3>& transform)
{
	id_type id = this->_entities3d.insert(Entity<3>{ this->_entities3d.next_id(), universe_id, transform });
	this->_universes3d[universe_id].add_entity(id);
	return this->_entities3d[id];
}


template <mv::uint dims, typename std::enable_if<dims == 2, int>::type>
mv::Universe<2>& mv::Multiverse::create_universe(
	uint cell_count_x, uint cell_count_y, float cell_size_x, float cell_size_y)
{
	id_type id = this->_universes2d.insert(
		Universe<2>{ this->_universes2d.next_id(), cell_count_x, cell_count_y, cell_size_x, cell_size_y });
	return this->_universes2d[id];
}

template <mv::uint dims, typename std::enable_if<dims == 3, int>::type>
mv::Universe<3>& mv::Multiverse::create_universe(
	uint cell_count_x, uint cell_count_y, uint cell_count_z, float cell_size_x, float cell_size_y, float cell_size_z)
{
	id_type id = this->_universes3d.insert(
		Universe<3>{ this->_universes3d.next_id(), cell_count_x, cell_count_y, cell_count_z, cell_size_x, cell_size_y, cell_size_z });
	return this->_universes3d[id];
}




template mv::Entity<2>& mv::Multiverse::entity<2>(id_type);
template mv::Universe<2>& mv::Multiverse::universe<2>(id_type);
template mv::Entity<2>& mv::Multiverse::create_entity<2>(id_type);
template mv::Entity<2>& mv::Multiverse::create_entity<2>(id_type, const Transform<2>&);
template mv::Universe<2>& mv::Multiverse::create_universe<2>(uint, uint, float, float);
template mv::Entity<3>& mv::Multiverse::entity<3>(id_type);
template mv::Universe<3>& mv::Multiverse::universe<3>(id_type);
template mv::Entity<3>& mv::Multiverse::create_entity<3>(id_type);
template mv::Entity<3>& mv::Multiverse::create_entity<3>(id_type, const Transform<3>&);
template mv::Universe<3>& mv::Multiverse::create_universe<3>(uint, uint, uint, float, float, float);
