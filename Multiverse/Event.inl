#include "Event.h"

#include <utility>	// forward


template <typename... EventArgs>
template <typename F, typename>
inline typename mv::detail::EventBase<EventArgs...>::id_type mv::detail::EventBase<EventArgs...>::attach(F&& callback)
{
	this->_callbacks.emplace(std::forward<F>(callback));
	return static_cast<id_type>(this->_callbacks.size()) - 1;
}

template <typename... EventArgs>
void mv::detail::EventBase<EventArgs...>::detach(typename mv::detail::EventBase<EventArgs...>::id_type id)
{
	this->_callbacks.erase(id);
}

template <typename... EventArgs>
void mv::detail::EventBase<EventArgs...>::clear()
{
	this->_callbacks.clear();
}


template <typename... EventArgs>
template <typename... Args, typename>
void mv::detail::EventBase<EventArgs...>::raise(Args&&... e) const
{
	for (const callback_type& callback : this->_callbacks) {
		callback(e...);
	}
}
