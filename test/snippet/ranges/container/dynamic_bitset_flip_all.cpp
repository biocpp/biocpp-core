#include <bio/meta/debug_stream.hpp>
#include <bio/ranges/container/dynamic_bitset.hpp>

int main()
{
    bio::dynamic_bitset t1{"10001100"};

    t1.flip();
    bio::debug_stream << t1 << '\n'; // 0111'0011
}
