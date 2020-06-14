#pragma once
#include "setup.h"
#include "CollisionShape.h"
#include "Event.h"

#include <set>

namespace mv
{
	template <uint dims>
	class Entity;

	enum class CollisionResponse : byte
	{
		ignore, overlap, block
	};
	enum class CollisionLayer : byte
	{
		layer1, layer2, layer3, layer4, layer5, layer6, layer7, layer8
	};

	template <uint dims>
	class Collider
	{
		friend Entity<dims>;

	private:
		CollisionShape<dims> _shape;
		CollisionLayer _layer;
		uint _response_mask;

		std::set<id_type> _overlaps;

	public:
		Event<Collider> begin_overlap;
		Event<Collider> end_overlap;
		Event<Collider> hit;

		void set_shape(const CollisionShape<dims>& shape);
		void set_shape(CollisionShape<dims>&& shape);
		void set_layer(CollisionLayer layer);
		void set_response(CollisionLayer layer, CollisionResponse response);

		CollisionLayer layer() const;
		CollisionResponse response(CollisionLayer layer) const;

		const std::set<id_type>& overlaps() const;
	};
}
