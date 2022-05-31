#include <bio/meta/debug_stream.hpp>
#include <bio/ranges/container/dynamic_bitset.hpp>

int main()
{
    bio::dynamic_bitset t1{0b1011'1000'1111};

    // Positions are 0-based and start at the rightmost bit.
    for (size_t i = 0; i < t1.size(); ++i)
        bio::debug_stream << t1[i]; // 1111'0001'1101

    bio::debug_stream << '\n';
}
