#include <bio/alphabet/fmt.hpp>
#include <bio/ranges/container/dynamic_bitset.hpp>

int main()
{
    bio::ranges::dynamic_bitset const t1{"10001100"};

    bio::ranges::dynamic_bitset t2 = t1 << 3;
    fmt::print("{}\n", t2); // 0110'0000
}
