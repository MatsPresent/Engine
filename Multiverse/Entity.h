#pragma once
#include "setup.h"

#include <iterator>
#include <vector>
#include <map>

namespace mv
{
	class Entity final
	{
		struct LookupEntry
		{
			unsigned short count;
			unsigned short idx;
		};

		template <typename ComponentType>
		class ComponentIterator : public std::iterator<std::random_access_iterator_tag, ComponentType>
		{
			id_type _universe_id;
			const id_type* _component_id;

		public:
			ComponentIterator() = default;
			ComponentIterator(id_type universe_id, const id_type* component_id);

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
			friend ComponentIterator<ComponentType> operator+(std::ptrdiff_t n, const ComponentIterator<ComponentType>& it);
			ComponentIterator<ComponentType>& operator-=(std::ptrdiff_t n);
			ComponentIterator<ComponentType> operator-(std::ptrdiff_t n) const;
			std::ptrdiff_t operator-(const ComponentIterator<ComponentType>& rhs) const;

			ComponentType& operator[](std::ptrdiff_t i) const;
		};
		
		template <typename ComponentType>
		class ComponentList
		{
			id_type _entity_id;
			LookupEntry _lookup_entry;

		public:
			ComponentIterator<ComponentType> begin() const;
			ComponentIterator<ComponentType> end() const;

			std::size_t size() const;
			bool empty() const;

			ComponentType& operator[](std::size_t i) const;
			ComponentType& at(std::size_t i) const;
		};


		id_type _id;
		id_type _universe_id; // id of the universe in which the entity resides
		std::vector<id_type> _component_idxs; // indices of the components in their respective lists inside the entity's universe
		std::map<const void*, LookupEntry> _component_lookup; // keeps count and first index of component id for each component type


	public:
		/**
			\brief get entity id
		*/
		id_type id() const;
		/**
			\brief get universe id
		*/
		id_type universe_id() const;

		/**
			\brief get component of type
			\returns an attached component of the specified type

			get a component of the specified type, if multiple components of that type are attached to this entity,
			there is no guarantee on which one it will return
		*/
		template <typename ComponentType>
		ComponentType& get() const;
		/**
			\brief get all components of type
			\returns a list of all attached components of the specified type
		*/
		template <typename ComponentType>
		ComponentList<ComponentType> get_all() const;
		/**
			\brief has component of type
		*/
		template <typename ComponentType>
		bool has() const;
		/**
			\brief total attached components
		*/
		std::size_t count() const;

		template <typename ComponentType, typename... ConstructorArgs>
		ComponentType& add(ConstructorArgs&&... constructor_args);
		void remove(id_type component_id);
	};
}
