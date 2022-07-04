#include <bio/alphabet/fmt.hpp>
#include <bio/ranges/container/dynamic_bitset.hpp>

int main()
{
    bio::ranges::dynamic_bitset t1{"10001100"};

    t1.reset();
    fmt::print("{}\n", t1); // 0000'0000
}
