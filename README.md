# Event Reactor

The goal is to make a hub object able to respond to arbitrary events
by calling registered callbacks corresponding to that event type.

## Issues

For now the EventReactor class is working, but it has an undesired dependency
on an Event class from which all event should derive.
Moreover, this dependency enters the code of EventReactor's methods without
being passed as argument.

We managed to correctly use events that don't derive from this base class.
That necessitates the use of reinterpret_cast inside the reactTo() method.
Yet we didn't get rid of the base class for now.
