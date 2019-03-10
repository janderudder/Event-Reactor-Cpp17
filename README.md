# Event Reactor

C++ event hub.
Fire callbacks when events are received.

An EventReactor instance registers any number of arbitrary types, each one associated with any number of callbacks.
When the EventReactor instance receive an event object, the callbacks registered for this object's type are called.

A callback can be a function, a static member function, or a member function (const or not). It must be of type `void()` or `void(const EventType&)`. The second variant makes it easy to process any kind of payload that the event object may contain.


## Example

```cpp
// Our event object
struct Event {} event;

// An object that will be called back
struct Object {
  void sayHi() const { std::cout << "Hello Object!\n"; }
} object;

// Instantiate our event hub
EventReactor reactor;

// Register a callback for events of type Event.
// The `sayHi()` method, bound to the instance `object`, is the callback.
reactor.registerCallback<Event>(Object::sayHi, object);

// Our hub receives the event
reactor.reactTo(event);

// The hub reacts by calling the registered callback.
// Output: Hello Object!
```

You can see other working examples in `main.cpp`.


## API

Here is the complete interface documentation.

### Construct

Default constructor (no parameter).

#### Example
```cpp
EventReactor reactor;
```


### Register a callback

```cpp
EntryLocation_t registerCallback<Event_Type>(Callback_T);
EntryLocation_t registerCallback<Event_Type>(Callback_T, SomeType&);
EntryLocation_t registerCallback<Event_Type>(Callback_T, SomeType const&);
```

The **template parameter** `Event_Type` may be any type. The registered callback will be called when an object of `Event_Type` is passed to the `EventReactor::reactTo()` method.

The three overloads of this method template take as **parameter(s)**, respectively:
  - a lambda, a free function or a class' static function
  - a non-const method of a class, and a non-const reference to an instance of this class
  - a const method of a class, and a const reference to an instance of this class

The reference to `SomeType` will be forwarded as the implicit `this` parameter of the callback method.

`Callback_T` may be anything that is callable with the right signature. It must have one of the two following forms:

```cpp
void callback();
void callback(const Event_Type&);
```

where `Event_Type` is the same as the template parameter of `registerCallback`. Referencing the event in the callback enables us to pass and process data with each event.

Lastly, the **return type** `EntryLocation_t` is an object pointing to the new record created in the EventReactor. It can be used with the `eraseEntry` method to remove this particular registration.


#### Usage example
```cpp
const auto entry = reactor.registerCallback<Event>(
  [&](const Event& event) {
    std::cout << event.getSomeString() << '\n';
});
```


### React to an event

```cpp
template <class T>
void reactTo(const T& event) const
```
This method takes a const reference to an object of any type.
If this type has been used as template parameter to the `registerCallback` method,
then all the corresponding callback functions will be called in registration order.

#### Example
```cpp
// Fire all callbacks registered with `registerCallback<decltype(event)>(...)`
reactor.reactTo(event);
```


### Remove a callback

```cpp
void eraseEntry(const EntryLocation_t& location)
```
Taking an EntryLocation object as returned by `registerCallback`, this method will
erase the corresponding entry in the underlying storage of the class.

#### Example
```cpp
const auto entryLocation = reactor.registerCallback<Event>(/*...*/);
reactor.eraseEntry(entryLocation);
```


## Implementation details

For the sake of interface simplicity, the callback calls are wrapped into another function call.
However no performance costs have been mesured on a x86 machine.


## License

Public domain. You are welcome to use, modify and redistribute this software.
However, as I'm curious, I would very well like to know if you actually use or improve it.

THIS SOFTWARE AND PROGRAM IS BEING PROVIDED "AS IS", WITHOUT ANY WARRANTY OF
ANY TYPE OR NATURE, EITHER EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE,
AND ANY WARRANTY THAT THIS SOFTWARE OR PROGRAM IS FREE FROM DEFECTS.
