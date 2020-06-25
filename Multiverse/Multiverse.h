#pragma once
#include "setup.h"

#include "IDList.h"
#include "ServiceLocator.h"

namespace mv
{
	class DebugService;
	class InputService;
	class Renderer;
	class ResourceManager;
	class ThreadPool;

	template <uint dims>
	class Transform;
	template <uint dims>
	class Entity;
	template <uint dims>
	class Universe;

	class Multiverse 
	{
	public:
		using service_locator_type = ServiceLocator<DebugService, InputService>;

		static const float tick_interval;
		static const uint tick_frequency;

	private:
		static service_locator_type _service_locator;

		static Renderer* _renderer;
		static ResourceManager* _resource_manager;
		static ThreadPool* _thread_pool;

		static IDList<Entity<2>, id_type> _entities2d;
		static IDList<Entity<3>, id_type> _entities3d;
		static IDList<Universe<2>, id_type> _universes2d;
		static IDList<Universe<3>, id_type> _universes3d;


		Multiverse() = delete;

	public:
		static void init();
		static void run();

		static inline const service_locator_type& service_locator() {
			return _service_locator;
		}
		static Renderer& renderer();
		static ResourceManager& resource_manager();
		static ThreadPool& thread_pool();

		template <uint dims, typename std::enable_if<dims == 2, int>::type = 0>
		static Entity<2>& entity(id_type id);
		template <uint dims, typename std::enable_if<dims == 3, int>::type = 0>
		static Entity<3>& entity(id_type id);

		template <uint dims, typename std::enable_if<dims == 2, int>::type = 0>
		static Universe<2>& universe(id_type id);
		template <uint dims, typename std::enable_if<dims == 3, int>::type = 0>
		static Universe<3>& universe(id_type id);

		template <uint dims, typename std::enable_if<dims == 2, int>::type = 0>
		static Entity<2>& create_entity(id_type universe_id);
		template <uint dims, typename std::enable_if<dims == 2, int>::type = 0>
		static Entity<2>& create_entity(id_type universe_id, const Transform<2>& transform, bool is_static = false);
		template <uint dims, typename std::enable_if<dims == 3, int>::type = 0>
		static Entity<3>& create_entity(id_type universe_id);
		template <uint dims, typename std::enable_if<dims == 3, int>::type = 0>
		static Entity<3>& create_entity(id_type universe_id, const Transform<3>& transform, bool is_static = false);

		template <uint dims, typename std::enable_if<dims == 2, int>::type = 0>
		static Universe<2>& create_universe(
			uint cell_count_x = MV_CELL_COUNT_DEFAULT, uint cell_count_y = MV_CELL_COUNT_DEFAULT,
			float cell_size_x = MV_CELL_SIZE_DEFAULT, float cell_size_y = MV_CELL_SIZE_DEFAULT);
		template <uint dims, typename std::enable_if<dims == 3, int>::type = 0>
		static Universe<3>& create_universe(
			uint cell_count_x = MV_CELL_COUNT_DEFAULT, uint cell_count_y = MV_CELL_COUNT_DEFAULT, uint cell_count_z = MV_CELL_COUNT_DEFAULT,
			float cell_size_x = MV_CELL_SIZE_DEFAULT, float cell_size_y = MV_CELL_SIZE_DEFAULT, float cell_size_z = MV_CELL_SIZE_DEFAULT);

	private:
		static void _cleanup();
	};
}