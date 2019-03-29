#include <inttypes.h>
#include <type_traits>

template <typename T>
T swap_16(const T x)
{
    static_assert(std::is_same<uint16_t,T>::value,"Input Type Error");
    return (((x&0xffu)<<8)|((x&0xff00u)>>8));
}

template <typename T>
T swap_32(const T x)
{
    static_assert(std::is_same<uint32_t,T>::value,"Input Type Error");
    return (((x&0xffu)<<24)|((x&0xff00u)<<8)|((x&0xff0000u)>>8)|((x&0xff000000u)>>24));
}

template <typename T>
T swap_64(const T x)
{
    static_assert(std::is_same<uint64_t,T>::value,"Input Type Error");
    return  (((x & 0xff00000000000000ull) >> 56)
           | ((x & 0x00ff000000000000ull) >> 40)
           | ((x & 0x0000ff0000000000ull) >> 24)
           | ((x & 0x000000ff00000000ull) >> 8)
           | ((x & 0x00000000ff000000ull) << 8)
           | ((x & 0x0000000000ff0000ull) << 24)
           | ((x & 0x000000000000ff00ull) << 40)
           | ((x & 0x00000000000000ffull) << 56));
}

// <endian.h>
// LITTLE_ENDIAN
// if (BYTE_ORDER == BIG_ENDIAN)
