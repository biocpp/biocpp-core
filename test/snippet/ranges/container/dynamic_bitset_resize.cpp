#include <bio/alphabet/fmt.hpp>
#include <bio/ranges/container/dynamic_bitset.hpp>

int main()
{
    bio::dynamic_bitset t1{"1100"};

    t1.resize(8);        // Enlarge to 8.
    fmt::print("{}\n", t1); // 0000'1100
    t1.resize(5);        // Shrink to 5, last three bits (5, 6, 7) are set to false.
    fmt::print("{}\n", t1); // 0110'0
    t1.resize(10, true); // Enlarge to 10 and set new bits to true.
    fmt::print("{}\n", t1); // 1111'1011'00
}
