#pragma once
#include <cstdint>
using TypeIdentifier = std::uintptr_t;

template <typename T>
constexpr TypeIdentifier getTypeId()
{
    return reinterpret_cast<TypeIdentifier>(&getTypeId<T>);
}
