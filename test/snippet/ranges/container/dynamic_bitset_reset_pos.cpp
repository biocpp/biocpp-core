#include <bio/meta/debug_stream.hpp>
#include <bio/ranges/container/dynamic_bitset.hpp>

int main()
{
    bio::dynamic_bitset t1{"10001100"};

    t1.reset(2);
    t1.reset(3);
    bio::debug_stream << t1 << '\n'; // 1000'0000
}
