/**
 * EventReactor
 *
 * Jan Derudder <janderudder.dev@gmail.com>
 *
 * License: Public Domain
 *
 * See bottom of this file for terms of use.
 *
 */
#pragma once
#include <map>
#include <vector>
#include <functional>
#include "getTypeId.hpp"


class EventReactor
{
    using EventType_t       = std::uintptr_t;
    using Callback_t        = std::function<void(const void*)>;
    using Map_t             = std::map<EventType_t, std::vector<Callback_t>>;
    using EntryLocation_t   = std::pair<Map_t::iterator, std::size_t>;

    Map_t mCallbacks;

public:
    /**
     * Register free and static class functions.
     */
    template <class Event_T, class Func_T>
    constexpr EntryLocation_t registerCallback(Func_T&& function);

    /**
     * Register non-const class methods taking const& to event as argument.
     */
    template <class Event_T, class T>
    constexpr EntryLocation_t registerCallback(void (T::*function)(const Event_T&), T& object);

    /**
     * Register const class methods taking const& to event as argument.
     */
    template <class Event_T, class T>
    constexpr EntryLocation_t registerCallback(void (T::*function)(const Event_T&) const, const T& object);

    /**
     * Register non-const class methods taking no argument.
     */
    template <class Event_T, class T>
    constexpr EntryLocation_t registerCallback(void (T::*function)(), T& object);

    /**
     * Register const class methods taking no argument.
     */
    template <class Event_T, class T>
    constexpr EntryLocation_t registerCallback(void (T::*function)() const, const T& object);

    /**
     * Delete a registered callback
     */
    void eraseEntry(const EntryLocation_t& location);

    /**
     * Fire callbacks registered for this type of event.
     */
    template <class Event_T>
    constexpr void reactTo(const Event_T& event) const;


private:
    /**
     * Used by `registerCallback` overloads to effectively insert
     * the callback into the instance's map and return the location
     * of the new entry.
     */
    EntryLocation_t _insertCallback(EventType_t eventType, std::function<void(const void*)>&& callback);


};



// Methods definitions are included here
#include "EventReactor.tpl.hpp"





/*
You are welcome to use, modify and redistribute this software.

THIS SOFTWARE AND PROGRAM IS BEING PROVIDED "AS IS", WITHOUT ANY WARRANTY OF
ANY TYPE OR NATURE, EITHER EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE,
AND ANY WARRANTY THAT THIS SOFTWARE OR PROGRAM IS FREE FROM DEFECTS.
*/
