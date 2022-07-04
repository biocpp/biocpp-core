#include <bio/alphabet/fmt.hpp>
#include <bio/ranges/container/dynamic_bitset.hpp>

int main()
{
    bio::ranges::dynamic_bitset t1{0b1011'1000'1111};

    // begin() refers to the rightmost position.
    for (auto it = t1.begin(); it != t1.end(); ++it)
        fmt::print("{}", *it); // 111100011101

    fmt::print("\n");
}
