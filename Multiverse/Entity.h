#pragma once
#include "setup.h"

#include <iterator> // iterator, random_access_iterator_tag
#include <map> // map
#include <type_traits> // enable_if, is_base_of
#include <vector> // vector

#include "UpdateStage.h"
#include "Multiverse.h"
#include "Transform.h"

namespace mv
{
	template <uint dims, UpdateStage stage>
	class Component;
	template <uint dims>
	class Universe;

	template <uint dims>
	class Entity final
	{
		friend Multiverse;

	public:
		using transform_type = Transform<dims>;

	private:
		template <typename ComponentType>
		class ComponentIterator : public std::iterator<std::random_access_iterator_tag, ComponentType>
		{
		public:
			using component_type = ComponentType;

		private:
			const id_type* _component_id_it;
			id_type _universe_id;

		public:
			ComponentIterator() = default;
			ComponentIterator(const id_type* component_id, id_type universe_id);

			bool operator==(const ComponentIterator<ComponentType>& rhs) const;
			bool operator!=(const ComponentIterator<ComponentType>& rhs) const;
			bool operator<(const ComponentIterator<ComponentType>& rhs) const;
			bool operator>(const ComponentIterator<ComponentType>& rhs) const;
			bool operator<=(const ComponentIterator<ComponentType>& rhs) const;
			bool operator>=(const ComponentIterator<ComponentType>& rhs) const;

			ComponentType& operator*() const;
			ComponentType* operator->() const;

			ComponentIterator<ComponentType>& operator++();
			ComponentIterator<ComponentType> operator++(int);
			ComponentIterator<ComponentType>& operator--();
			ComponentIterator<ComponentType> operator--(int);

			ComponentIterator<ComponentType>& operator+=(std::ptrdiff_t n);
			ComponentIterator<ComponentType> operator+(std::ptrdiff_t n) const;
			ComponentIterator<ComponentType>& operator-=(std::ptrdiff_t n);
			ComponentIterator<ComponentType> operator-(std::ptrdiff_t n) const;
			std::ptrdiff_t operator-(const ComponentIterator<ComponentType>& rhs) const;

			ComponentType& operator[](std::ptrdiff_t i) const;
		};
		
		template <typename ComponentType>
		class ComponentList
		{
		public:
			using component_type = ComponentType;

		private:
			const id_type* _component_ids;
			size_type _component_count;
			id_type _universe_id;

			ComponentList(const id_type* ids, size_type count, id_type universe_id);

		public:
			type_id_type type_id() const;

			ComponentIterator<ComponentType> begin() const;
			ComponentIterator<ComponentType> end() const;

			size_type size() const;
			bool empty() const;

			ComponentType& operator[](mv::size_type i) const;
			ComponentType& at(mv::size_type i) const;
		};


		id_type _id; // id of this entity, unique in the multiverse
		id_type _universe_id; // id of the universe in which the entity resides

		transform_type _transform;
		transform_type _velocity;

		std::map<type_id_type, std::vector<id_type>> _component_ids; // unique ids of attached components per component type


		Entity(id_type id, id_type universe_id);

	public:
		Entity(const Entity&) = delete;
		Entity(Entity&& other) noexcept;

		Entity& operator=(const Entity&) = delete;
		Entity& operator=(Entity&& other) noexcept;

		/**
			\brief get entity id
		*/
		id_type id() const;
		/**
			\brief get universe id
		*/
		id_type universe_id() const;
		/**
			\brief get universe
		*/
		Universe<dims>& universe() const;

		const transform_type& get_transform() const;
		const transform_type& get_velocity() const;
		void set_transform(const transform_type& transform);
		void set_velocity(const transform_type& velocity);

		/**
			\brief get component of type
			\returns an attached component of the specified type, nullptr if not found

			Get a component of the specified type, if multiple components of that type are attached to this entity,
			there is no guarantee which one it will return. If no component of that type was found this throws an exception.
		*/
		template <typename ComponentType>
		ComponentType& component() const;
		/**
			\brief get all components of type
			\returns a list of all attached components of the specified type
		*/
		template <typename ComponentType>
		ComponentList<ComponentType> components() const;
		/**
			\brief get component of type
			\returns pointer to an attached component of the specified type, nullptr if not found

			Get a pointer to a component of the specified type, if multiple components of that type are attached to this entity,
			there is no guarantee which one it will return. If no component of that type was found this returns nullptr.
		*/
		template <typename ComponentType>
		ComponentType* find_component() const;

		template <typename ComponentType, typename... Args>
		ComponentType& add_component(Args&&... args);
		template <typename ComponentType>
		bool remove_component(id_type component_id);
	};

	template <uint dims, typename ComponentType>
	typename Entity<dims>::template ComponentIterator<ComponentType> operator+(
		std::ptrdiff_t n, const typename Entity<dims>::template ComponentIterator<ComponentType>& it);


	using Entity2D = Entity<2>;
	using Entity3D = Entity<3>;
}

#include "Entity.inl"
