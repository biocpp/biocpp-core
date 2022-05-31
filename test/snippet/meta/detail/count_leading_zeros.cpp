#include <bio/meta/bit_manipulation.hpp>
#include <bio/meta/debug_stream.hpp>

int main()
{
    uint8_t  t0 = 0b0000'1001;
    uint16_t t1 = 0b0100'0001'0000'1001;
    uint32_t t2 = 0b0000'0000'0000'0000'1000'0000'0000'0000;
    uint64_t t3 = 0b0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0001;

    bio::debug_stream << bio::detail::count_leading_zeros(t0) << '\n'; // 4
    bio::debug_stream << bio::detail::count_leading_zeros(t1) << '\n'; // 1
    bio::debug_stream << bio::detail::count_leading_zeros(t2) << '\n'; // 16
    bio::debug_stream << bio::detail::count_leading_zeros(t3) << '\n'; // 63

    return 0;
}
