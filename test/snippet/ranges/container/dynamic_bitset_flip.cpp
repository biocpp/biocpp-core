#include <bio/meta/debug_stream.hpp>
#include <bio/ranges/container/dynamic_bitset.hpp>

int main()
{
    bio::dynamic_bitset const t1{"10001100"};
    bio::dynamic_bitset t2 = ~t1;

    bio::debug_stream << t2 << '\n'; // 0111'0011
}
