#pragma once
#include "setup.h"

#include <vector>
#include <map>

#include "UpdateStage.h"
#include "Transform.h"

namespace mv
{
	class Multiverse;

	template <uint dims, UpdateStage stage>
	class Component;

	template <uint dims>
	class Entity;

	template <uint dims>
	class Universe final
	{
		friend Multiverse;
		friend Entity<dims>;

	public:
		using transform_type = Transform<dims>;
		using position_type = decltype(transform_type::translate);

	private:
		template <UpdateStage stage>
		class ComponentUpdaterBase
		{
		public:
			ComponentUpdaterBase() = default;
			virtual ~ComponentUpdaterBase() = default;

			virtual type_id_type type_id() const = 0;
			virtual std::size_t size() const = 0;

			virtual Component<dims, stage>& at(std::size_t i) = 0;
			virtual const Component<dims, stage>& at(std::size_t i) const = 0;

			virtual void remove(id_type id) = 0;
			
			virtual void update(float deltaTime) = 0;
			virtual void render() const = 0;
		};

		template <typename ComponentType>
		class ComponentUpdater final : public ComponentUpdaterBase<ComponentType::update_stage>
		{
			std::vector<ComponentType> _components;
			static std::vector<uint> _lookup; // maps component id to index in _components
			static std::vector<id_type> _freed_ids; // constant time access for freed component ids

		public:
			ComponentUpdater() = default;

			type_id_type type_id() const override;
			std::size_t size() const override;

			Component<dims, ComponentType::update_stage>& at(std::size_t i) override;
			const Component<dims, ComponentType::update_stage>& at(std::size_t i) const override;

			ComponentType& get(id_type id);
			const ComponentType& get(id_type id) const;

			ComponentType& add(ComponentType&& component);
			void remove(id_type id) override;

			void update(float deltaTime) override;
			void render() const override;
		};

		template <UpdateStage stage>
		class ComponentUpdaterList final
		{
			std::vector<ComponentUpdaterBase<stage>*> _updaters;
			std::map<type_id_type, unsigned int> _lookup; // updater index per component type

		public:
			ComponentUpdaterList() = default;
			ComponentUpdaterList(const ComponentUpdaterList<stage>&) = delete;
			ComponentUpdaterList(ComponentUpdaterList<stage>&& other) noexcept;

			~ComponentUpdaterList();

			ComponentUpdaterList<stage>& operator=(const ComponentUpdaterList<stage>&) = delete;
			ComponentUpdaterList<stage>& operator=(ComponentUpdaterList<stage>&& other) noexcept;

			ComponentUpdaterBase<stage>* const* begin() const;
			ComponentUpdaterBase<stage>* const* end() const;

			template <typename ComponentType>
			ComponentType& get(id_type component_id) const;

			template <typename ComponentType>
			ComponentType& add(ComponentType&& component);
			void remove(type_id_type component_type_id, id_type component_id);
		};

		class Gridspace
		{
		private:
			struct Cell
			{
				std::vector<id_type> static_entity_ids;
				std::vector<id_type> dynamic_entity_ids;
				uint count;
			};

			Cell* _cells;
			uint _cell_counts[dims]; // amount of cells allocated for each dimension
			float _cell_sizes[dims]; // sizes of cells for each dimension

		public:
			template <uint _ = dims, typename std::enable_if<_ == 2, int>::type = 0>
			Gridspace(uint cell_count_x, uint cell_count_y, float cell_size_x, float cell_size_y);
			template <uint _ = dims, typename std::enable_if<_ == 3, int>::type = 0>
			Gridspace(uint cell_count_x, uint cell_count_y, uint cell_count_z, float cell_size_x, float cell_size_y, float cell_size_z);
			Gridspace(const Gridspace&) = delete;
			Gridspace(Gridspace&& other) noexcept;

			~Gridspace();

			Gridspace& operator=(const Gridspace&) = delete;
			Gridspace& operator=(Gridspace&& other) noexcept;

			void add(id_type entity_id);
			void remove(id_type entity_id);

			template <uint _ = dims, typename std::enable_if<_ == 2, int>::type = 0>
			void update_cells();
			template <uint _ = dims, typename std::enable_if<_ == 3, int>::type = 0>
			void update_cells();

			template <uint _ = dims, typename std::enable_if<_ == 2, int>::type = 0>
			void update_collision() const;
			template <uint _ = dims, typename std::enable_if<_ == 3, int>::type = 0>
			void update_collision() const;

			template <uint _ = dims, typename std::enable_if<_ == 2, int>::type = 0>
			std::vector<Entity<2>*> entities_in_range(const position_type& origin, float radius) const;
			template <uint _ = dims, typename std::enable_if<_ == 3, int>::type = 0>
			std::vector<Entity<3>*> entities_in_range(const position_type& origin, float radius) const;

		private:
			template <uint _ = dims, typename std::enable_if<_ == 2, int>::type = 0>
			uint _cell_count() const;
			template <uint _ = dims, typename std::enable_if<_ == 3, int>::type = 0>
			uint _cell_count() const;

			template <uint _ = dims, typename std::enable_if<_ == 2, int>::type = 0>
			uint _calculate_cell(const position_type& position) const;
			template <uint _ = dims, typename std::enable_if<_ == 3, int>::type = 0>
			uint _calculate_cell(const position_type& position) const;
			uint _calculate_grid_coord(float world_coord, uint coord_idx) const;
		};


		id_type _id;

		Gridspace _gridspace;

		ComponentUpdaterList<UpdateStage::physics> _physics_updaters;
		ComponentUpdaterList<UpdateStage::postphysics> _postphysics_updaters;
		ComponentUpdaterList<UpdateStage::input> _input_updaters;
		ComponentUpdaterList<UpdateStage::behaviour> _behaviour_updaters;
		ComponentUpdaterList<UpdateStage::prerender> _prerender_updaters;
		ComponentUpdaterList<UpdateStage::render> _render_updaters;

		float _update_interval;
		float _render_interval;
		float _update_timeout;
		float _render_timeout;
		bool _update_enabled;
		bool _render_enabled;
		
		bool _transform_readonly;
		bool _transform_read_buffer;


		template <uint _ = dims, typename std::enable_if<_ == 2, int>::type = 0>
		Universe(id_type id, uint cell_count_x, uint cell_count_y, float cell_size_x, float cell_size_y);
		template <uint _ = dims, typename std::enable_if<_ == 3, int>::type = 0>
		Universe(id_type id, uint cell_count_x, uint cell_count_y, uint cell_count_z, float cell_size_x, float cell_size_y, float cell_size_z);

		void add_entity(id_type entity_id);
		void remove_entity(id_type entity_id);

		template <typename ComponentType, typename std::enable_if<std::is_base_of<Component<dims, UpdateStage::physics>, ComponentType>::value, int>::type = 0>
		ComponentType& get_component(id_type component_id) const;
		template <typename ComponentType, typename std::enable_if<std::is_base_of<Component<dims, UpdateStage::postphysics>, ComponentType>::value, int>::type = 0>
		ComponentType& get_component(id_type component_id) const;
		template <typename ComponentType, typename std::enable_if<std::is_base_of<Component<dims, UpdateStage::input>, ComponentType>::value, int>::type = 0>
		ComponentType& get_component(id_type component_id) const;
		template <typename ComponentType, typename std::enable_if<std::is_base_of<Component<dims, UpdateStage::behaviour>, ComponentType>::value, int>::type = 0>
		ComponentType& get_component(id_type component_id) const;
		template <typename ComponentType, typename std::enable_if<std::is_base_of<Component<dims, UpdateStage::prerender>, ComponentType>::value, int>::type = 0>
		ComponentType& get_component(id_type component_id) const;
		template <typename ComponentType, typename std::enable_if<std::is_base_of<Component<dims, UpdateStage::render>, ComponentType>::value, int>::type = 0>
		ComponentType& get_component(id_type component_id) const;

		template <typename ComponentType, typename std::enable_if<std::is_base_of<Component<dims, UpdateStage::physics>, ComponentType>::value, int>::type = 0>
		ComponentType& add_component(ComponentType&& component);
		template <typename ComponentType, typename std::enable_if<std::is_base_of<Component<dims, UpdateStage::postphysics>, ComponentType>::value, int>::type = 0>
		ComponentType& add_component(ComponentType&& component);
		template <typename ComponentType, typename std::enable_if<std::is_base_of<Component<dims, UpdateStage::input>, ComponentType>::value, int>::type = 0>
		ComponentType& add_component(ComponentType&& component);
		template <typename ComponentType, typename std::enable_if<std::is_base_of<Component<dims, UpdateStage::behaviour>, ComponentType>::value, int>::type = 0>
		ComponentType& add_component(ComponentType&& component);
		template <typename ComponentType, typename std::enable_if<std::is_base_of<Component<dims, UpdateStage::prerender>, ComponentType>::value, int>::type = 0>
		ComponentType& add_component(ComponentType&& component);
		template <typename ComponentType, typename std::enable_if<std::is_base_of<Component<dims, UpdateStage::render>, ComponentType>::value, int>::type = 0>
		ComponentType& add_component(ComponentType&& component);
		template <typename ComponentType, typename std::enable_if<std::is_base_of<Component<dims, UpdateStage::physics>, ComponentType>::value, int>::type = 0>
		void remove_component(id_type component_id);
		template <typename ComponentType, typename std::enable_if<std::is_base_of<Component<dims, UpdateStage::postphysics>, ComponentType>::value, int>::type = 0>
		void remove_component(id_type component_id);
		template <typename ComponentType, typename std::enable_if<std::is_base_of<Component<dims, UpdateStage::input>, ComponentType>::value, int>::type = 0>
		void remove_component(id_type component_id);
		template <typename ComponentType, typename std::enable_if<std::is_base_of<Component<dims, UpdateStage::behaviour>, ComponentType>::value, int>::type = 0>
		void remove_component(id_type component_id);
		template <typename ComponentType, typename std::enable_if<std::is_base_of<Component<dims, UpdateStage::prerender>, ComponentType>::value, int>::type = 0>
		void remove_component(id_type component_id);
		template <typename ComponentType, typename std::enable_if<std::is_base_of<Component<dims, UpdateStage::render>, ComponentType>::value, int>::type = 0>
		void remove_component(id_type component_id);
		void remove_component(UpdateStage stage, type_id_type component_type_id, id_type component_id);

		void update(float delta_time);
		void render(float delta_time);

	public:
		Universe(const Universe<dims>&) = delete;
		Universe(Universe<dims>&& other) noexcept;

		Universe& operator=(const Universe<dims>&) = delete;
		Universe& operator=(Universe<dims>&& other) noexcept;

		/**
			\brief get universe id
		*/
		id_type id() const;

		Entity<dims>& spawn_entity(const transform_type& transform = transform_type{}) const;

		void set_update_interval(float interval);
		void set_update_enabled(bool enabled);
		void set_render_interval(float interval);
		void set_render_enabled(bool enabled);
	};


	using Universe2D = Universe<2>;
	using Universe3D = Universe<3>;
}

#include "Universe.inl"
