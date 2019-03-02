#pragma once
#include <iostream>
#include "Event.hpp"


struct MyStruct {

    float x, y;

    MyStruct() : x{0}, y{0} {}

    void inc() {
        x += 1.f;
        y += 1.f;

        std::cout << "inc() : " << this->x << ", " << this->y << "\n";
    }

    void react(const Event& ev) {
        x += reinterpret_cast<const ResizeEvent&>(ev).x;
        y += reinterpret_cast<const ResizeEvent&>(ev).y;
    }
};
