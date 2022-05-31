#include <bio/meta/debug_stream.hpp>
#include <bio/ranges/container/dynamic_bitset.hpp>

int main()
{
    seqan3::dynamic_bitset t1{"10001100"};

    t1.set(0);
    t1.set(2, false);
    seqan3::debug_stream << t1 << '\n'; // 1000'1001
}
