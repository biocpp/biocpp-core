#include <bio/meta/bit_manipulation.hpp>
#include <bio/alphabet/fmt.hpp>

int main()
{
    uint8_t  t0 = 0b0000'1001;
    uint16_t t1 = 0b0100'0001'0000'1000;
    uint32_t t2 = 0b0000'0000'0000'0000'1000'0000'0000'0000;
    uint64_t t3 = 0b0000'0000'0000'0000'0010'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000;

    fmt::print("{}\n", bio::detail::count_trailing_zeros(t0)); // 0
    fmt::print("{}\n", bio::detail::count_trailing_zeros(t1)); // 3
    fmt::print("{}\n", bio::detail::count_trailing_zeros(t2)); // 15
    fmt::print("{}\n", bio::detail::count_trailing_zeros(t3)); // 45

    return 0;
}
