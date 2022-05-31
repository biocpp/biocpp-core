#include <bio/meta/debug_stream.hpp>
#include <bio/ranges/container/dynamic_bitset.hpp>

int main()
{
    bio::dynamic_bitset t1{"10001100"};        // Construct from string.
    bio::dynamic_bitset const t2{0b1011'1000}; // Construct from binary literal or integer.

    bio::debug_stream << t1 << '\n';           // Debug printing inserts separators: 1000'1100
    std::cout << t2 << '\n';                      // Standard printing does not: 10111000

    t1 &= t2;                                     // Assign t1 the result of a binary AND of t1 and t2.
    bio::debug_stream << t1 << '\n';           // 1000'1000
}
