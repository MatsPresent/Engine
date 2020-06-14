#pragma once
#include "setup.h"

#include <functional>	// function
#include <utility>	// declval
#include <vector>	// vector

#include "IDList.h"

namespace mv
{
	template <typename... EventArgs>
	class EventBase
	{
	public:
		using callback_type = std::function<void(EventArgs...)>;
		using id_type = unsigned int;

	private:
		IDList<callback_type, id_type> _callbacks;

	public:
		/**
			\brief Attach a callback
			\param callback a callable object
			\returns id of newly attached callback

			The callback object must be invokable with arguments of types EventArgs&&...
		*/
		template <typename F, typename = decltype(callback_type(std::declval<F>()))>
		id_type attach(F&& callback);
		/**
			\brief Detach a callback
			\param id Id of the to be detached callback
			\returns detached callback if successful
		*/
		void detach(id_type id);
		/**
			\brief Detach all callbacks
		*/
		void clear();

	protected:
		/**
			\brief Invoke callback functions
		*/
		void raise(EventArgs&&... e) const;
	};

	template <typename OwnerType, typename... EventArgs>
	class Event : public EventBase<EventArgs...>
	{
		friend OwnerType;

	public:
		using EventBase<EventArgs...>::callback_type;
		using EventBase<EventArgs...>::id_type;

	private:
		using EventBase<EventArgs...>::raise;
	};

	template <typename... EventArgs>
	class Event<void, EventArgs...> : public EventBase<EventArgs...>
	{
	public:
		using EventBase<EventArgs...>::callback_type;
		using EventBase<EventArgs...>::id_type;

		using EventBase<EventArgs...>::raise;
	};
}

#include "Event.inl"
