#pragma once
#include <any>
#include <map>
#include <vector>
#include <functional>

#include "getTypeId.hpp"


class EventReactor
{
    using EventType_t       = TypeIdentifier;
    using Callback_t        = std::function<void(const std::any&)>;
    using Map_t             = std::map<EventType_t, std::vector<Callback_t>>;
    using EntryLocation_t   = std::pair<Map_t::iterator, std::size_t>;


    Map_t mCallbacks;


public:
    /**
     * Register free and static class functions.
     */
    template <class Event_T, class Func_T>
    constexpr EntryLocation_t registerCallback(Func_T&& function)
    {
        if constexpr (std::is_convertible_v<Func_T, std::function<void()>>) {
            return this->_insertCallback(getTypeId<Event_T>(), [&](const std::any&) {
                function();
            });
        }
        else return this->_insertCallback(getTypeId<Event_T>(), [&](const std::any& ev) {
            function(std::any_cast<const Event_T&>(ev));
        });
    }

    /**
     * Register non-const class methods taking const& to event as argument.
     */
    template <class Event_T, class T>
    constexpr EntryLocation_t registerCallback(void (T::*function)(const Event_T&), T& object)
    {
        auto memFun = std::mem_fn(function);
        auto callback = [memFun, &function, &object](const std::any& ev) {
            std::invoke(memFun, object, std::any_cast<const Event_T&>(ev));
        };
        return this->_insertCallback(getTypeId<Event_T>(), std::move(callback));
    }

    /**
     * Register const class methods taking const& to event as argument.
     */
    template <class Event_T, class T>
    constexpr EntryLocation_t registerCallback(void (T::*function)(const Event_T&) const, const T& object)
    {
        auto memFun = std::mem_fn(function);
        auto callback = [memFun, &function, &object](const std::any& ev) {
            std::invoke(memFun, object, std::any_cast<const Event_T&>(ev));
        };
        return this->_insertCallback(getTypeId<Event_T>(), std::move(callback));
    }

    /**
     * Register non-const class methods taking no argument.
     */
    template <class Event_T, class T>
    constexpr EntryLocation_t registerCallback(void (T::*function)(), T& object)
    {
        auto memFun = std::mem_fn(function);
        auto callback = [memFun, &function, &object](const std::any&) {
            std::invoke(memFun, object);
        };
        return this->_insertCallback(getTypeId<Event_T>(), std::move(callback));
    }

    /**
     * Register const class methods taking no argument.
     */
    template <class Event_T, class T>
    constexpr EntryLocation_t registerCallback(void (T::*function)() const, const T& object)
    {
        auto memFun = std::mem_fn(function);
        auto callback = [memFun, &function, &object](const std::any&) {
            std::invoke(memFun, object);
        };
        return this->_insertCallback(getTypeId<Event_T>(), std::move(callback));
    }


    void eraseEntry(const EntryLocation_t& location)
    {
        // Find the right vector.
        const auto [iterator, index] = location;
        auto& vector = iterator->second;

        // Find the right entry.
        auto vecIt = vector.begin();
        std::advance(vecIt, index);

        vector.erase(vecIt);
    }


    /**
     * Fire callbacks registered for this type of event.
     */
    template <class Event_T>
    constexpr void reactTo(const Event_T& event) const
    {
        auto it = mCallbacks.find(getTypeId<Event_T>());

        // Bail if no callbacks registered.
        if (it == mCallbacks.end())
            return;

        // Call all callbacks for this event type id.
        for (const auto& callback : std::as_const(it->second)) {
            std::invoke(callback, std::any{event});
        }
    }


private:
    /**
     * Used by `registerCallback` overloads to effectively insert
     * the callback into the instance's map and return the location
     * of the new entry.
     */
    EntryLocation_t _insertCallback(EventType_t eventType, std::function<void(const std::any&)>&& callback)
    {
        // Get an iterator to the relevant array in the map.
        // It will be constructed in place now if it is not already there.
        std::pair<Map_t::iterator, bool>
            insertionResult = mCallbacks.try_emplace(eventType);

        // The iterator gives us the array to insert to.
        auto& vector = insertionResult.first->second;
        vector.push_back(std::move(callback));

        // Construct the returned object, which enables finding this entry later.
        return { insertionResult.first, vector.size() - 1 };
    }


};
