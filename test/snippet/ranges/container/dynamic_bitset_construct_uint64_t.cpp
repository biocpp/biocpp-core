#include <bio/meta/debug_stream.hpp>
#include <bio/ranges/container/dynamic_bitset.hpp>

int main()
{
    bio::dynamic_bitset t1{0b1011'1000'1111}; // Use binary literal.
    bio::dynamic_bitset t2{0b0000'1000'1111}; // Leading zeros are stripped.
    bio::dynamic_bitset t3{832};              // Use a number.

    bio::debug_stream << t1 << '\n'; // 1011'1000'1111
    bio::debug_stream << t2 << '\n'; // 1000'1111
    bio::debug_stream << t3 << '\n'; // 1101'0000'00
}
