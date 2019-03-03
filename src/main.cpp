#include <iostream>
#include <variant>

#include "EventReactor.hpp"

struct MyEvent {};
struct ResizeEvent
{
    float x, y;

    ResizeEvent(float xx, float yy) noexcept
        : x     {xx}
        , y     {yy}
    {}
};

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

void freeFunc() { std::cout << "Hello free function.\n"; }

void freeFunc2(const ResizeEvent& ev) {
    std::cout << "freeFunc2: " << ev.x << ", " << ev.y << "\n";
}




int main()
{

    MyEvent myEvent;
    MyStruct myStruct;
    ResizeEvent resizeEvent{1.1, 2.2};
    EventReactor er;

    er.registerCallback<MyEvent>([&]{ std::cout << "Hello MyEvent.\n"; });
    er.registerCallback<MyEvent>(freeFunc);
    er.registerCallback<ResizeEvent>(freeFunc2);
    er.registerCallback<ResizeEvent>(&MyStruct::inc, myStruct);
    er.registerCallback<ResizeEvent>(MyStruct::react, myStruct);
    er.registerCallback<ResizeEvent>(MyStruct::show, myStruct);

    er.reactTo(myEvent);
    er.reactTo(resizeEvent);
}
