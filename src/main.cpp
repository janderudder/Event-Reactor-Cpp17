/**
 **     Examples for EventReactor
 **
 **     author: Jan Derudder
 **     license: MIT
 **
 **/
#include <iostream>
#include <variant>

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


// * Free functions
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
    reactor.registerCallback<MyEvent>(freeFunc);

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

    // Fire the callbacks for both types
    reactor.reactTo(myEvent);
    reactor.reactTo(resizeEvent);
}
