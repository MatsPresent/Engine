#pragma once
#include "setup.h"
#include "Entity.h"
struct SDL_Window;

namespace mv
{
	class Multiverse 
	{
		friend Entity<2>;
		friend Entity<3>;

	public:
		static const float tick_interval;
		static const unsigned int tick_frequency;

	private:
		SDL_Window* _window;
		Entity<2>* _entities2d;
		Entity<3>* _entities3d;
		Universe<2>* _universes2d;
		Universe<3>* _universes3d;


		Multiverse();


	public:
		static Multiverse& get();

		void init();
		void cleanup();
		void run();

		template <unsigned int dims, typename std::enable_if<dims == 2, int>::type = 0>
		Entity<2>& entity(id_type id);
		template <unsigned int dims, typename std::enable_if<dims == 3, int>::type = 0>
		Entity<3>& entity(id_type id);
		Entity2D& entity2d(id_type id);
		Entity3D& entity3d(id_type id);

	private:
		template <unsigned int dims, typename std::enable_if<dims == 2, int>::type = 0>
		Universe<2>& _universe(id_type id);
		template <unsigned int dims, typename std::enable_if<dims == 3, int>::type = 0>
		Universe<3>& _universe(id_type id);
	};
}