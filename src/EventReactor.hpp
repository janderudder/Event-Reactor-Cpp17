#pragma once
#include <map>
#include <functional>
#include <vector>
#include <cstdint>
#include <variant>
#include <iterator>
#include <type_traits>

#include "Event.hpp"
#include "getTypeId.hpp"
/**
 * class EventReactor
 *
 * Register an arbitrary number of functions to be executed when a certain type
 * of event is received by the reactor.
 *
 * Two types of callbacks can be stored by this class:
 *
 *      - functions with no argument `void ()`
 *      - functions taking a pointer to Event as argument `void (const Event*)`
 *
 * A callback can be a free function (for example when passing a lambda),
 * or a member function, in which case it must be bound to an object instance:
 * the implicit `this` first argument of the method.
 *
 * This class maps event categories (id of type) to arrays of callbacks.
 * To enable the erasure of a callback from the vector, for example when a
 * corresponding object is destroyed, and therefore avoid dereferencing freed
 * memory, the `registerCallback` method will return an object containing the
 * information needed to locate the new entry at a later time.
 *
 * This object is a `std::pair<iterator, index>`, where `first` is an iterator
 * in the reactor's map (pointing to a vector), and `second` is the index of
 * the entry in the vector. An object can store and use this pair to delete its
 * related entry inside the EventReactor, via the `unsubscribe` method.
 *
 */
class EventReactor
{
    using EventType_T       = TypeIdentifier;
    using Callback_T        = std::function<void(const Event&)>;
    using Map_T             = std::map<EventType_T, std::vector<Callback_T>>;
    using EntryLocation_T   = std::pair<Map_T::iterator, std::size_t>;


    Map_T mCallbacks;


public:
    /**
     * Register free and static class functions.
     */
    template <class Event_T, class Func_T>
    EntryLocation_T registerCallback(Func_T&& function)
    {
        // Wrap the function in the storable function type if needed
        auto callback = ([&function] {
            if constexpr (!std::is_convertible_v<Func_T, std::function<void(const Event&)>>) {
                return [&function](const Event&){function();};
            }
            else return function;
        })();

        return this->_insertCallback(getTypeId<Event_T>(), std::move(callback));
    }


    /**
     * Register class methods taking const Event& as argument.
     */
    template <class Event_T, class T>
    EntryLocation_T registerCallback(void (T::*function)(const Event&), T& object)
    {
        auto callback = std::bind(function, &object, std::placeholders::_1);
        return this->_insertCallback(getTypeId<Event_T>(), std::move(callback));
    }


    /**
     * Register class methods taking no argument.
     */
    template <class Event_T, class T>
    EntryLocation_T registerCallback(void (T::*function)(), T& object)
    {
        auto callback = std::bind(function, &object);
        return this->_insertCallback(getTypeId<Event_T>(), std::move(callback));
    }


    void eraseEntry(const EntryLocation_T& location)
    {
        // Find the right vector.
        const auto [iterator, index] = location;
        auto& vector = iterator->second;

        // Find right entry.
        auto vecIt = vector.begin();
        std::advance(vecIt, index);

        vector.erase(vecIt);
    }


    /**
     * Fire callbacks registered for this type of event.
     */
    template <class Event_T>
    void reactTo(const Event_T& event) const
    {
        auto it = mCallbacks.find(getTypeId<Event_T>());

        // Bail if no callbacks registered.
        if (it == mCallbacks.end())
            return;

        // Call all callbacks for this event type id.
        for (const auto& callback : std::as_const(it->second)) {
            std::invoke(callback, event);
        }
    }


private:
    /**
     * Used by registerCallback(...) overloads to effectively insert
     * the callback into the instance's map and return the location
     * of the new entry.
     */
    EntryLocation_T _insertCallback(EventType_T eventType, std::function<void(const Event&)>&& callback)
    {
        // Get an iterator to the relevant array in the map (it is
        // constructed in place now if it is not already in there).
        std::pair<Map_T::iterator, bool>
            insertionResult = mCallbacks.try_emplace(eventType);

        // A reference to the array, taken from the iterator.
        auto& vector = insertionResult.first->second;

        // Construct the new std::variant with the 'no arg' callback type.
        vector.push_back(std::move(callback));

        // Construct the returned object, to enable finding this entry later.
        return { insertionResult.first, vector.size() - 1 };
    }

};
