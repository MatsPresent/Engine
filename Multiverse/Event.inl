#include "Event.h"

#include <algorithm>	// find
#include <utility>	// forward



template <typename... EventArgs>
template <typename F, typename>
inline typename mv::EventBase<EventArgs...>::id_type mv::EventBase<EventArgs...>::attach(F&& callback)
{
	this->_callbacks.emplace(std::forward<F>(callback));
	return static_cast<id_type>(this->_callbacks.size()) - 1;
}

template <typename... EventArgs>
void mv::EventBase<EventArgs...>::detach(typename mv::EventBase<EventArgs...>::id_type id)
{
	this->_callbacks.erase(id);
}

template <typename... EventArgs>
void mv::EventBase<EventArgs...>::clear()
{
	this->_callbacks.clear();
}


template <typename... EventArgs>
void mv::EventBase<EventArgs...>::raise(EventArgs&&... e) const
{
	for (const callback_type& callback : this->_callbacks) {
		callback(e...); // don't forward args, moving them would invalidate that arg for subsequent callbacks
	}
}
