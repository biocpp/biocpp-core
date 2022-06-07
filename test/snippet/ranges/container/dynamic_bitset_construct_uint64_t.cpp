#include <bio/alphabet/fmt.hpp>
#include <bio/ranges/container/dynamic_bitset.hpp>

int main()
{
    bio::dynamic_bitset t1{0b1011'1000'1111}; // Use binary literal.
    bio::dynamic_bitset t2{0b0000'1000'1111}; // Leading zeros are stripped.
    bio::dynamic_bitset t3{832};              // Use a number.

    fmt::print("{}\n", t1); // 1011'1000'1111
    fmt::print("{}\n", t2); // 1000'1111
    fmt::print("{}\n", t3); // 1101'0000'00
}
