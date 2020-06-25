#include "MultiversePCH.h"
#include "Multiverse.h"

#include <chrono>
#include <type_traits>

#include "Entity.h"
#include "Universe.h"

#include "Renderer.h"
#include "ResourceManager.h"
#include "ThreadPool.h"

#include "Transform.h"

#include "Debug.h"
#include "ConsoleLogger.h"

#include "Input.h"
#include "SDLInputHandler.h"


#ifndef MV_TICKFREQUENCY
#define MV_TICKFREQUENCY 60
#endif // !MG_TICKFREQUENCY
static_assert(std::is_integral<decltype(MV_TICKFREQUENCY)>::value && MV_TICKFREQUENCY > 0,
	"[mv] MV_TICKFREQUENCY must be a positive integral value");
static_assert(MV_TICKFREQUENCY <= 65536, "[mv] Tick frequency cannot exceed 65536 Hz");


const float mv::Multiverse::tick_interval = static_cast<float>(1'000'000'000 / MV_TICKFREQUENCY) / 1'000'000'000;
const mv::uint mv::Multiverse::tick_frequency = MV_TICKFREQUENCY;

mv::Multiverse::service_locator_type mv::Multiverse::_service_locator;

mv::Renderer* mv::Multiverse::_renderer;
mv::ResourceManager* mv::Multiverse::_resource_manager;
mv::ThreadPool* mv::Multiverse::_thread_pool;

mv::IDList<mv::Entity<2>, mv::id_type> mv::Multiverse::_entities2d;
mv::IDList<mv::Entity<3>, mv::id_type> mv::Multiverse::_entities3d;
mv::IDList<mv::Universe<2>, mv::id_type> mv::Multiverse::_universes2d;
mv::IDList<mv::Universe<3>, mv::id_type> mv::Multiverse::_universes3d;



const mv::ServiceProxy<mv::DebugService> mv::debug(mv::Multiverse::service_locator().get<mv::DebugService>());
const mv::ServiceProxy<mv::InputService> mv::input(mv::Multiverse::service_locator().get<mv::InputService>());




void mv::Multiverse::init()
{
	_service_locator.set<DebugService, ConsoleLogger>();
	_service_locator.set<InputService, SDLInputHandler>();
	Renderer::Settings renderer_settings{};
	renderer_settings.window.title = "Window Title";
	renderer_settings.window.width = 640;
	renderer_settings.window.height = 480;
	_thread_pool = new ThreadPool(std::min(1, static_cast<int>(std::thread::hardware_concurrency()) - 1));
	_renderer = new Renderer(renderer_settings);
	_resource_manager = new ResourceManager("../Data/");
}

void mv::Multiverse::run()
{
	constexpr std::chrono::high_resolution_clock::duration tick_duration(
		std::chrono::duration_cast<std::chrono::high_resolution_clock::duration>(
		std::chrono::duration<long long, std::nano>(1'000'000'000 / tick_frequency)));

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
			exit = _service_locator.get<InputService>()->update() || exit;
			for (Universe<2>& universe : _universes2d) {
				universe.update(tick_interval);
			}
			for (Universe<3>& universe : _universes3d) {
				universe.update(tick_interval);
			}
			behind_time -= tick_duration;
		}

		for (Universe<2>& universe : _universes2d) {
			universe.render(frame_interval);
		}
		for (Universe<3>& universe : _universes3d) {
			universe.render(frame_interval);
		}
		_renderer->render();
	}

	_cleanup();
}


mv::Renderer& mv::Multiverse::renderer()
{
	return *_renderer;
}

mv::ResourceManager& mv::Multiverse::resource_manager()
{
	return *_resource_manager;
}

mv::ThreadPool& mv::Multiverse::thread_pool()
{
	return *_thread_pool;
}


template <mv::uint dims, typename std::enable_if<dims == 2, int>::type>
inline mv::Entity<2>& mv::Multiverse::entity(id_type id)
{
	return _entities2d[id];
}

template <mv::uint dims, typename std::enable_if<dims == 3, int>::type>
inline mv::Entity<3>& mv::Multiverse::entity(id_type id)
{
	return _entities3d[id];
}


template <mv::uint dims, typename std::enable_if<dims == 2, int>::type>
mv::Universe<2>& mv::Multiverse::universe(id_type id)
{
	return _universes2d[id];
}

template <mv::uint dims, typename std::enable_if<dims == 3, int>::type>
mv::Universe<3>& mv::Multiverse::universe(id_type id)
{
	return _universes3d[id];
}


template <mv::uint dims, typename std::enable_if<dims == 2, int>::type>
mv::Entity<2>& mv::Multiverse::create_entity(id_type universe_id)
{
	id_type id = _entities2d.insert(Entity<2>{ _entities2d.next_id(), universe_id, Transform<2>{}, false });
	_universes2d[universe_id].add_entity(id);
	return _entities2d[id];
}

template <mv::uint dims, typename std::enable_if<dims == 2, int>::type>
mv::Entity<2>& mv::Multiverse::create_entity(id_type universe_id, const Transform<2>& transform, bool is_static)
{
	id_type id = _entities2d.insert(Entity<2>{ _entities2d.next_id(), universe_id, transform, is_static });
	_universes2d[universe_id].add_entity(id);
	return _entities2d[id];
}

template <mv::uint dims, typename std::enable_if<dims == 3, int>::type>
mv::Entity<3>& mv::Multiverse::create_entity(id_type universe_id)
{
	id_type id = _entities3d.insert(Entity<3>{ _entities3d.next_id(), universe_id, Transform<3>{}, false });
	_universes3d[universe_id].add_entity(id);
	return _entities3d[id];
}

template <mv::uint dims, typename std::enable_if<dims == 3, int>::type>
mv::Entity<3>& mv::Multiverse::create_entity(id_type universe_id, const Transform<3>& transform, bool is_static)
{
	id_type id = _entities3d.insert(Entity<3>{ _entities3d.next_id(), universe_id, transform, is_static });
	_universes3d[universe_id].add_entity(id);
	return _entities3d[id];
}


template <mv::uint dims, typename std::enable_if<dims == 2, int>::type>
mv::Universe<2>& mv::Multiverse::create_universe(
	uint cell_count_x, uint cell_count_y, float cell_size_x, float cell_size_y)
{
	id_type id = _universes2d.insert(
		Universe<2>{ _universes2d.next_id(), cell_count_x, cell_count_y, cell_size_x, cell_size_y });
	return _universes2d[id];
}

template <mv::uint dims, typename std::enable_if<dims == 3, int>::type>
mv::Universe<3>& mv::Multiverse::create_universe(
	uint cell_count_x, uint cell_count_y, uint cell_count_z, float cell_size_x, float cell_size_y, float cell_size_z)
{
	id_type id = _universes3d.insert(
		Universe<3>{ _universes3d.next_id(), cell_count_x, cell_count_y, cell_count_z, cell_size_x, cell_size_y, cell_size_z });
	return _universes3d[id];
}



void mv::Multiverse::_cleanup()
{
	delete _resource_manager;
	delete _renderer;
	delete _thread_pool;
}




template mv::Entity<2>& mv::Multiverse::entity<2>(id_type);
template mv::Universe<2>& mv::Multiverse::universe<2>(id_type);
template mv::Entity<2>& mv::Multiverse::create_entity<2>(id_type);
template mv::Entity<2>& mv::Multiverse::create_entity<2>(id_type, const Transform<2>&, bool);
template mv::Universe<2>& mv::Multiverse::create_universe<2>(uint, uint, float, float);
template mv::Entity<3>& mv::Multiverse::entity<3>(id_type);
template mv::Universe<3>& mv::Multiverse::universe<3>(id_type);
template mv::Entity<3>& mv::Multiverse::create_entity<3>(id_type);
template mv::Entity<3>& mv::Multiverse::create_entity<3>(id_type, const Transform<3>&, bool);
template mv::Universe<3>& mv::Multiverse::create_universe<3>(uint, uint, uint, float, float, float);
