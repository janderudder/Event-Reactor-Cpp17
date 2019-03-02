#pragma once
#include "getTypeId.hpp"


struct Event {
    const TypeIdentifier    type;
    const void*             data;

    Event(const TypeIdentifier id, const void* pData = nullptr)
        : type      {id}
        , data      {pData}
    {}
};


struct ResizeEvent : public Event
{
    float x, y;

    ResizeEvent(float xx, float yy) noexcept
        : Event {getTypeId<ResizeEvent>(), this}
        , x     {xx}
        , y     {yy}
    {}
};
