#include <bio/meta/debug_stream.hpp>
#include <bio/ranges/container/dynamic_bitset.hpp>

int main()
{
    bio::dynamic_bitset t1{0b1011'1000'1111};

    // begin() refers to the rightmost position.
    for (auto it = t1.begin(); it != t1.end(); ++it)
        bio::debug_stream << *it; // 1111'0001'1101

    bio::debug_stream << '\n';
}
