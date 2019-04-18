/**
 * Methods definitions for EventReactor class template.
 */
#include "EventReactor.hpp" // suppress errors in the IDE

/**
 * Register free and static class functions.
 */
template <class Event, class Fn>
EventReactor::EntryLocation EventReactor::registerCallback
(
    Fn&& function
)
{
    if constexpr (std::is_convertible_v<Fn, std::function<void()>>)
    { // If callback has no parameter
        return this->_insertCallback(this->_getKey<Event>(), [&](const void*) {
            function();
        });
    }
    else return this->_insertCallback(this->_getKey<Event>(), [&](const void* ev) {
        function(*static_cast<const Event*>(ev));
    });
}

/**
 * Register non-const class methods taking const& to event as argument.
 */
template <class Event, class T>
EventReactor::EntryLocation EventReactor::registerCallback
(
    MemFnEventParam<Event, T> function,
    T& object
)
{
    auto memFun = std::mem_fn(std::move(function));
    auto callback = [memFun, &function, &object](const void* ev) {
        std::invoke(memFun, &object, *static_cast<const Event*>(ev));
    };
    return this->_insertCallback(this->_getKey<Event>(), std::move(callback));
}

/**
 * Register const class methods taking const& to event as argument.
 */
template <class Event, class T>
EventReactor::EntryLocation EventReactor::registerCallback
(
    ConstMemFnEventParam<Event, T> function,
    const T& object
)
{
    auto memFun = std::mem_fn(std::move(function));
    auto callback = [memFun, &function, &object](const void* ev) {
        std::invoke(memFun, &object, *static_cast<const Event*>(ev));
    };
    return this->_insertCallback(this->_getKey<Event>(), std::move(callback));
}

/**
 * Register non-const class methods taking no argument.
 */
template <class Event, class T>
EventReactor::EntryLocation EventReactor::registerCallback
(
    MemFnNoParam<T> function,
    T& object
)
{
    auto memFun = std::mem_fn(std::move(function));
    auto callback = [memFun, &function, &object](const void*) {
        std::invoke(memFun, &object);
    };
    return this->_insertCallback(this->_getKey<Event>(), std::move(callback));
}

/**
 * Register const class methods taking no argument.
 */
template <class Event, class T>
EventReactor::EntryLocation EventReactor::registerCallback
(
    ConstMemFnNoParam<T> function,
    const T& object
)
{
    auto memFun = std::mem_fn(std::move(function));
    auto callback = [memFun, &function, &object](const void*) {
        std::invoke(memFun, &object);
    };
    return this->_insertCallback(this->_getKey<Event>(), std::move(callback));
}

/**
 * Delete a registered callback
 */
void EventReactor::eraseEntry(const EntryLocation& location)
{
    mCallbacks.erase(location);
}

/**
 * Fire callbacks registered for this type of event.
 */
template <class Event>
void EventReactor::reactTo(const Event& event) const
{
    const auto range = mCallbacks.equal_range(this->_getKey<Event>());

    for (auto it = range.first; it != range.second; ++it) {
        std::invoke(it->second, static_cast<const void*>(&event));
    }
}

/**
 * Private utility method
 * Used by `registerCallback` overloads to effectively insert
 * the callback into the instance's map and return the location
 * of the new entry.
 */
EventReactor::EntryLocation EventReactor::_insertCallback
(
    Key eventType,
    std::function<void(const void*)>&& callback
)
{
    return mCallbacks.insert({eventType, std::move(callback)});
}

/**
 * Transform a type into a key for the multimap.
 */
template <class Event>
EventReactor::Key EventReactor::_getKey() const
{
    return std::type_index(typeid(Event));
}




/*
You are welcome to use, modify and redistribute this software.

THIS SOFTWARE AND PROGRAM IS BEING PROVIDED "AS IS", WITHOUT ANY WARRANTY OF
ANY TYPE OR NATURE, EITHER EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE,
AND ANY WARRANTY THAT THIS SOFTWARE OR PROGRAM IS FREE FROM DEFECTS.
*/
