#pragma once
#include <chrono>
#include <cstddef>
#include <iostream>
#include "EventReactor.hpp"

#define LOOP_COUNT 1000000


void benchmark()
{
    struct Event {
        int i; double d;
    };
    struct Val {
        int i = 0; double d = 0;
        void update(const Event& ev) {
            i += ev.i; d += ev.d;
        }
    };

    Event ev{1, 0.1};
    Val val;

    std::cout << "val: " << val.i << ", " << val.d << "\n";

    std::chrono::time_point tStart = std::chrono::high_resolution_clock::now();

    EventReactor er;

    er.registerCallback<Event>(Val::update, val);
    er.registerCallback<Event>([&](const Event& e){val.i += e.i; val.d += e.d;});

    for (std::size_t n=0; n < LOOP_COUNT; ++n) {
        er.reactTo(ev);
    }

    const auto tEnd = std::chrono::high_resolution_clock::now();

    std::cout << "val: " << val.i << ", " << val.d << "\n";
    std::cout << "bench time for " << LOOP_COUNT << " loop rounds : " << std::chrono::duration_cast<std::chrono::milliseconds>(tEnd - tStart).count() << " milliseconds.\n";
}
