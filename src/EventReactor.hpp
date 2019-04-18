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
#include <typeinfo>
#include <typeindex>


class EventReactor
{
    // Declare the field of this class
    using Key               = std::type_index;
    using Callback          = std::function<void(const void*)>;
    using Multimap          = std::multimap<Key, Callback>;

    Multimap mCallbacks;

    // Additional useful types
    using EntryLocation         = Multimap::iterator;

    template <class T>
    using MemFnNoParam          = void (T::*)();

    template <class T>
    using ConstMemFnNoParam     = void (T::*)() const;

    template <class Event, class T>
    using MemFnEventParam       = void (T::*)(const Event&);

    template <class Event, class T>
    using ConstMemFnEventParam  = void (T::*)(const Event&) const;


public:
    /**
     * Register free and static class functions.
     */
    template <class Event, class Fn>
    EntryLocation registerCallback(Fn&&);

    /**
     * Register non-const class methods taking const& to event as argument.
     */
    template <class Event, class T>
    EntryLocation registerCallback(MemFnEventParam<Event, T>, T& object);

    /**
     * Register const class methods taking const& to event as argument.
     */
    template <class Event, class T>
    EntryLocation registerCallback(ConstMemFnEventParam<Event, T>, const T& object);

    /**
     * Register non-const class methods taking no argument.
     */
    template <class Event, class T>
    EntryLocation registerCallback(MemFnNoParam<T>, T& object);

    /**
     * Register const class methods taking no argument.
     */
    template <class Event, class T>
    EntryLocation registerCallback(ConstMemFnNoParam<T>, const T& object);

    /**
     * Delete a registered callback
     */
    void eraseEntry(const EntryLocation& location);

    /**
     * Fire callbacks registered for this type of event.
     */
    template <class Event>
    void reactTo(const Event& event) const;


private:
    /**
     * Used by `registerCallback` overloads to effectively insert
     * the callback into the instance's map and return the location
     * of the new entry.
     */
    EntryLocation _insertCallback(Key, std::function<void(const void*)>&&);

    /**
     * Transform a type into a key for the multimap.
     */
    template <class Event>
    Key _getKey() const;

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
