#include <bio/meta/debug_stream.hpp>
#include <bio/ranges/container/dynamic_bitset.hpp>

int main()
{
    bio::dynamic_bitset t1{"10001100"};
    bio::dynamic_bitset const t2{0b1011'1000};

    t1 &= t2;
    bio::debug_stream << t1 << '\n'; // 1000'1000
}
