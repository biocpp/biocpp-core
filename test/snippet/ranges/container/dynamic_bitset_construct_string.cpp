#include <bio/alphabet/fmt.hpp>
#include <bio/ranges/container/dynamic_bitset.hpp>

int main()
{
    bio::ranges::dynamic_bitset t1{"101110001111"}; // Use character literal.
    bio::ranges::dynamic_bitset t2{"000010001111"}; // Leading zeros are kept.

    fmt::print("{}\n", t1); // 1011'1000'1111
    fmt::print("{}\n", t2); // 0000'1000'1111
}
