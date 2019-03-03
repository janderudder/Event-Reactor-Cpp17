#pragma once
#include <cstdint>
/**
 * A little trick to get a unique integer for each template type.
 * The credit goes to Stellaris and the Internets.
 */
template <typename T>
constexpr std::uintptr_t getTypeId()
{
    return reinterpret_cast<std::uintptr_t>(&getTypeId<T>);
}
