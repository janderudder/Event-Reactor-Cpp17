# Event Reactor

C++ event hub.
Fire callbacks when events are received.

An EventReactor instance registers any number of arbitrary types / callbacks pairs.
When it is told to react to an event, it calls all the callbacks registered for this particular event type.

## API

Here is a minimal documentation. Please look at `main.cpp` for examples.

### Construct

Default constructor (no parameter).

### Register a callback

```cpp
EntryLocation_t registerCallback<Event_Type>(Callback_T);
EntryLocation_t registerCallback<Event_Type>(Callback_T, Obj&);
EntryLocation_t registerCallback<Event_Type>(Callback_T, Obj const&);
```

The three overloads of this method template take respectively
  - a lambda, free function or class's static function
  - a non-const method of a class Obj, and a non-const reference to an instance of Obj
  - a const method of Obj, and a const reference to an instance of Obj

`Callback_T` has some constraints : it can only return `void`, and it may take
either no parameter at all, either one parameter being a const reference to the
type passed as template parameter.

The instance objects passed in the overloads taking methods will be forwarded as
`this` parameters of the aforementioned methods.

### React to an event

```cpp
template <class T>
void reactTo(const T& event) const
```
This method takes a const reference to an object of any type.
If this type has already been used as template parameter to the `registerCallback` method,
then all the corresponding registered functions will be called in registration order.

### Remove a callback

```cpp
void eraseEntry(const EntryLocation_t& location)
```
Taking an EntryLocation object as returned by `registerCallback`, this method will
erase the corresponding entry in the underlying storage of the class.

## Implementation details

For the sake of (some level of) genericity and the simplicity of the interface, the callback calls are wrapped into another function call.
This doubles the number of function calls and probably have performance costs.

The callbacks are very limited in return and parameter types. If you know a way to obtain a completely generic interface I would be interested to know. Please contact me.

## License

You are welcome to use and redistribute this software.
However, as I'm curious, I would very well like to know if you actually use or improve it.

THIS SOFTWARE AND PROGRAM IS BEING PROVIDED "AS IS", WITHOUT ANY WARRANTY OF
ANY TYPE OR NATURE, EITHER EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE,
AND ANY WARRANTY THAT THIS SOFTWARE OR PROGRAM IS FREE FROM DEFECTS.
