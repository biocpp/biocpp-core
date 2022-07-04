#include <bio/alphabet/fmt.hpp>
#include <bio/ranges/container/dynamic_bitset.hpp>

int main()
{
    bio::ranges::dynamic_bitset t1{"10001100"};
    bio::ranges::dynamic_bitset const t2{0b1011'1000};

    t1 &= t2;
    fmt::print("{}\n", t1); // 1000'1000
}
