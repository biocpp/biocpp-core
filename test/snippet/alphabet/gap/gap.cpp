#include <iostream>

#include <bio/alphabet/gap/gap.hpp>
#include <bio/alphabet/fmt.hpp>

int main()
{
    bio::gap my_gap = bio::gap{};
    bio::gap another_gap{};
    another_gap.assign_char('A'); // this does not change anything

    fmt::print("{}", my_gap.to_char()); // outputs '-'
    if (my_gap.to_char() == another_gap.to_char())
        fmt::print("Both gaps are the same!");
}
