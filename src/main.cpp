/*
 *   Examples for EventReactor
 *
 *   Author: Jan Derudder <janderudder.dev@gmail.com>
 *   License: Public Domain
 *
 *   See bottom of this file for terms of use.
 *
 */
#include <iostream>
#include "EventReactor.hpp"


//* Empty arbitrary event
struct MyEvent {};

//* Event with data
struct ResizeEvent
{
    float x, y;

    ResizeEvent(float xx, float yy) noexcept
        : x     {xx}
        , y     {yy}
    {}
};

//* Struct that will be accessed by callbacks
//* (methods called and data written & read).
struct MyStruct {

    float x, y;

    MyStruct() : x{0}, y{0} {}

    void show() const {
        std::cout << "State of myStruct now : " << x << ", " << y << "\n";
    }

    void inc() {
        x += 1.f;
        y += 1.f;

        std::cout << "inc() : " << this->x << ", " << this->y << "\n";
    }

    void react(const ResizeEvent& ev) {
        x += ev.x;
        y += ev.y;
    }
};


//* Free functions
void freeFunc() { std::cout << "Hello free function.\n"; }

void freeFunc2(const ResizeEvent& ev) {
    std::cout << "freeFunc2: " << ev.x << ", " << ev.y << "\n";
}




int main()
{
    // Structure to play with
    MyStruct myStruct;

    // The main class of this program
    EventReactor reactor;

    // Lambda as callback
    reactor.registerCallback<MyEvent>([&]{ std::cout << "Hello MyEvent.\n"; });

    // Function without parameter
    auto loc = reactor.registerCallback<MyEvent>(freeFunc);

    // Function that receives the event as param
    reactor.registerCallback<ResizeEvent>(freeFunc2);

    // Method no param
    reactor.registerCallback<ResizeEvent>(&MyStruct::inc, myStruct);

    // Method with event as param
    reactor.registerCallback<ResizeEvent>(MyStruct::react, myStruct);

    // Const method, no param
    reactor.registerCallback<ResizeEvent>(MyStruct::show, myStruct);

    // Instantiate  events
    MyEvent myEvent;
    ResizeEvent resizeEvent{1.1, 2.2};

    // Remove the first free function
    reactor.eraseEntry(loc);

    // Fire the callbacks for both types
    reactor.reactTo(myEvent);
    reactor.reactTo(resizeEvent);

    /*
     * Expected output:
     *
     *     Hello MyEvent.
     *     freeFunc2: 1.1, 2.2
     *     inc() : 1, 1
     *     State of myStruct now : 2.1, 3.2
     */

}


/*

You are welcome to use and redistribute this software.

THIS SOFTWARE AND PROGRAM IS BEING PROVIDED "AS IS", WITHOUT ANY WARRANTY OF
ANY TYPE OR NATURE, EITHER EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE,
AND ANY WARRANTY THAT THIS SOFTWARE OR PROGRAM IS FREE FROM DEFECTS.

*/
