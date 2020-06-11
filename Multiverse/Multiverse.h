#pragma once
#include "setup.h"
#include "IDList.h"
struct SDL_Window;

namespace mv
{
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
		static const float tick_interval;
		static const uint tick_frequency;

	private:
		SDL_Window* _window;
		ResourceManager* _resource_manager;
		ThreadPool* _thread_pool;

		IDList<Entity<2>, id_type> _entities2d;
		IDList<Entity<3>, id_type> _entities3d;
		IDList<Universe<2>, id_type> _universes2d;
		IDList<Universe<3>, id_type> _universes3d;


		Multiverse();

	public:
		~Multiverse();

		static Multiverse& get();

		void init();
		void cleanup();
		void run();

		ResourceManager& resource_manager() const;
		ThreadPool& thread_pool() const;

		template <uint dims, typename std::enable_if<dims == 2, int>::type = 0>
		Entity<2>& entity(id_type id);
		template <uint dims, typename std::enable_if<dims == 3, int>::type = 0>
		Entity<3>& entity(id_type id);

		template <uint dims, typename std::enable_if<dims == 2, int>::type = 0>
		Universe<2>& universe(id_type id);
		template <uint dims, typename std::enable_if<dims == 3, int>::type = 0>
		Universe<3>& universe(id_type id);

		template <uint dims, typename std::enable_if<dims == 2, int>::type = 0>
		Entity<2>& create_entity(id_type universe_id);
		template <uint dims, typename std::enable_if<dims == 2, int>::type = 0>
		Entity<2>& create_entity(id_type universe_id, const Transform<2>& transform);
		template <uint dims, typename std::enable_if<dims == 3, int>::type = 0>
		Entity<3>& create_entity(id_type universe_id);
		template <uint dims, typename std::enable_if<dims == 3, int>::type = 0>
		Entity<3>& create_entity(id_type universe_id, const Transform<3>& transform);

		template <uint dims, typename std::enable_if<dims == 2, int>::type = 0>
		Universe<2>& create_universe(
			uint cell_count_x = MV_CELL_COUNT_DEFAULT, uint cell_count_y = MV_CELL_COUNT_DEFAULT,
			float cell_size_x = MV_CELL_SIZE_DEFAULT, float cell_size_y = MV_CELL_SIZE_DEFAULT);
		template <uint dims, typename std::enable_if<dims == 3, int>::type = 0>
		Universe<3>& create_universe(
			uint cell_count_x = MV_CELL_COUNT_DEFAULT, uint cell_count_y = MV_CELL_COUNT_DEFAULT, uint cell_count_z = MV_CELL_COUNT_DEFAULT,
			float cell_size_x = MV_CELL_SIZE_DEFAULT, float cell_size_y = MV_CELL_SIZE_DEFAULT, float cell_size_z = MV_CELL_SIZE_DEFAULT);
	};
}