#include <bio/meta/debug_stream.hpp>
#include <bio/ranges/container/dynamic_bitset.hpp>

int main()
{
    bio::dynamic_bitset t1{"101110001111"}; // Use character literal.
    bio::dynamic_bitset t2{"000010001111"}; // Leading zeros are kept.

    bio::debug_stream << t1 << '\n'; // 1011'1000'1111
    bio::debug_stream << t2 << '\n'; // 0000'1000'1111
}
