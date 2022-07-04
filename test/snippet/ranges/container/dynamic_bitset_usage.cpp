#include <bio/alphabet/fmt.hpp>
#include <bio/ranges/container/dynamic_bitset.hpp>

int main()
{
    bio::ranges::dynamic_bitset t1{"10001100"};         // Construct from string.
    bio::ranges::dynamic_bitset const t2{0b1011'1000};  // Construct from binary literal or integer.

    fmt::print("{}\n", t1);                     // Debug printing inserts separators: 1000'1100
    fmt::print("{}\n", t2);                     // Standard printing does not: 10111000

    t1 &= t2;                                   // Assign t1 the result of a binary AND of t1 and t2.
    fmt::print("{}\n", t1);                     // 1000'1000
}
