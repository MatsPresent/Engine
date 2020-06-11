#pragma once
#include "setup.h"

#include "UpdateStage.h"
#include "Universe.h"
#include "Matrix.h"

namespace mv
{
	template <uint dims>
	class Entity;

	template <uint dims, UpdateStage stage>
	class Component
	{
		friend class Universe<dims>;

		id_type _id; // id of component, unique per component type in the multiverse
		id_type _entity_id; // id of owning entity

	public:
		static constexpr UpdateStage update_stage = stage;

	protected:
		Component() = default;

	public:
		id_type id() const;
		id_type entity_id() const;
		id_type universe_id() const;
		Entity<dims>& entity() const;
		Universe<dims>& universe() const;

		void update(float delta_time);
	};

	template <uint dims>
	class Component<dims, UpdateStage::render>
	{
		friend class Universe<dims>;

		id_type _id; // id of component, unique per component type in the multiverse
		id_type _entity_id; // id of owning entity

	public:
		static constexpr UpdateStage update_stage = UpdateStage::render;

		Matrix<float, dims + 1, dims + 1> transform; // model transform matrix

	protected:
		Component() = default;

	public:
		id_type id() const;
		id_type entity_id() const;
		id_type universe_id() const;
		Entity<dims>& entity() const;
		Universe<dims>& universe() const;

		void update(float delta_time);
		void render() const;
	};


	template <UpdateStage stage>
	using Component2D = Component<2, stage>;
	template <UpdateStage stage>
	using Component3D = Component<3, stage>;
}
