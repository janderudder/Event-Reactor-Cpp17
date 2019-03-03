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



/*

You are welcome to use and redistribute this software.

THIS SOFTWARE AND PROGRAM IS BEING PROVIDED "AS IS", WITHOUT ANY WARRANTY OF
ANY TYPE OR NATURE, EITHER EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE,
AND ANY WARRANTY THAT THIS SOFTWARE OR PROGRAM IS FREE FROM DEFECTS.

*/
