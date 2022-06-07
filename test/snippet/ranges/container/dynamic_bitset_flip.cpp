#include <bio/alphabet/fmt.hpp>
#include <bio/ranges/container/dynamic_bitset.hpp>

int main()
{
    bio::dynamic_bitset const t1{"10001100"};
    bio::dynamic_bitset t2 = ~t1;

    fmt::print("{}\n", t2); // 0111'0011
}
