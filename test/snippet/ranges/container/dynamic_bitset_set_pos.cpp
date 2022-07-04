#include <bio/alphabet/fmt.hpp>
#include <bio/ranges/container/dynamic_bitset.hpp>

int main()
{
    bio::ranges::dynamic_bitset t1{"10001100"};

    t1.set(0);
    t1.set(2, false);
    fmt::print("{}\n", t1); // 1000'1001
}
