#pragma once
#include "setup.h"

#include "UpdateStage.h"
#include "Universe.h"
#include "Transform.h"
#include "Matrix.h"

namespace mv
{
	template <unsigned int dims>
	class Entity;

	template <unsigned int dims, UpdateStage stage>
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
		const Universe<dims>& universe() const;

		void update(float deltaTime);
	};

	template <unsigned int dims>
	class Component<dims, UpdateStage::physics>
	{
		friend class Universe<dims>;

		id_type _id; // id of this component, unique per component type in the multiverse
		id_type _entity_id; // id of owning entity
		Transform<dims> _transform;
		Transform<dims> _velocity;

	public:
		static constexpr UpdateStage update_stage = UpdateStage::physics;

	protected:
		Component() = default;

	public:
		id_type id() const;
		id_type entity_id() const;
		id_type universe_id() const;
		const Entity<dims>& entity() const;
		const Universe<dims>& universe() const;
		Transform<dims>& transform();
		const Transform<dims>& transform() const;
		Transform<dims>& velocity();
		const Transform<dims>& velocity() const;

		void update(float deltaTime);
	};

	template <unsigned int dims>
	class Component<dims, UpdateStage::render>
	{
		friend class Universe<dims>;

		id_type _id; // id of component, unique per component type in the multiverse
		id_type _entity_id; // id of owning entity
		Matrix<dims + 1> _transform; // model transform matrix

	public:
		static constexpr UpdateStage update_stage = UpdateStage::render;

	protected:
		Component() = default;

	public:
		id_type id() const;
		id_type entity_id() const;
		id_type universe_id() const;
		const Entity<dims>& entity() const;
		const Universe<dims>& universe() const;
		const Matrix<dims + 1>& transform() const;

		void update(float deltaTime);
		void render() const;
	};


	template <UpdateStage stage>
	using Component2D = Component<2, stage>;
	template <UpdateStage stage>
	using Component3D = Component<3, stage>;
}
