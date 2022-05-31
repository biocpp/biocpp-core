#include <iostream>                     // for std::cout
#include <bio/alphabet/all.hpp>      // include all alphabet headers

template <bio::alphabet t>
void print(t const v)
{
    std::cout << "I am an alphabet and my value as char is: " << bio::to_char(v) << '\n';
}

using bio::operator""_dna5;
using bio::operator""_aa27;

int main()
{
    auto d = 'A'_dna5;
    auto a = 'L'_aa27;
    auto g = bio::gap{};

    print(d);
    print(a);
    print(g);
}
