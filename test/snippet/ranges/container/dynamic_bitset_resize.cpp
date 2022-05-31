#include <bio/meta/debug_stream.hpp>
#include <bio/ranges/container/dynamic_bitset.hpp>

int main()
{
    bio::dynamic_bitset t1{"1100"};

    t1.resize(8);        // Enlarge to 8.
    bio::debug_stream << t1 << '\n'; // 0000'1100
    t1.resize(5);        // Shrink to 5, last three bits (5, 6, 7) are set to false.
    bio::debug_stream << t1 << '\n'; // 0110'0
    t1.resize(10, true); // Enlarge to 10 and set new bits to true.
    bio::debug_stream << t1 << '\n'; // 1111'1011'00
}
