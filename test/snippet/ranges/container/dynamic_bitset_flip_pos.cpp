#include <bio/alphabet/fmt.hpp>
#include <bio/ranges/container/dynamic_bitset.hpp>

int main()
{
    bio::ranges::dynamic_bitset t1{"10001100"};

    t1.flip(0);
    t1.flip(3);
    fmt::print("{}\n", t1); // 1000'0101
}
