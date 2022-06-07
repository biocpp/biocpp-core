#include <bio/meta/bit_manipulation.hpp>
#include <bio/alphabet/fmt.hpp>

int main()
{
    uint8_t  t0 = 0b0000'1001;
    uint16_t t1 = 0b0100'0001'0000'1001;
    uint32_t t2 = 0b0000'0000'0000'0000'0000'0000'0000'0001;
    uint64_t t3 = 0b0100'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000;

    fmt::print("{}\n", bio::detail::most_significant_bit_set(t0)); // 3
    fmt::print("{}\n", bio::detail::most_significant_bit_set(t1)); // 14
    fmt::print("{}\n", bio::detail::most_significant_bit_set(t2)); // 0
    fmt::print("{}\n", bio::detail::most_significant_bit_set(t3)); // 62

    return 0;
}
