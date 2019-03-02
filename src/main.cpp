#include <iostream>
#include <variant>

#include "EventReactor.hpp"
#include "Event.hpp"
#include "MyStruct.hpp"
#include "getTypeId.hpp"

void freeFunc() { std::cout << "Hello free function.\n"; }

void freeFunc2(const Event& ev) {
    auto rev = static_cast<const ResizeEvent&>(ev);
    std::cout << "freeFunc2: " << rev.x << ", " << rev.y << "\n";
}


struct MyEvent : public Event {
    // const TypeIdentifier    type    =   getTypeId<MyEvent>();
    // const void*             data    =   nullptr;
    MyEvent() : Event{getTypeId<MyEvent>()} {}
};


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
    er.registerCallback<ResizeEvent>(&MyStruct::react, myStruct);
    er.registerCallback<ResizeEvent>([&]{std::cout << myStruct.x << ", " << myStruct.y << "\n";});

    er.reactTo(myEvent);
    er.reactTo(resizeEvent);
}
