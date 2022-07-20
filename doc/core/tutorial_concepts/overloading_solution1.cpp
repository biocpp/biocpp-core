#include <iostream>                     // for std::cout
#include <bio/alphabet/all.hpp>      // include all alphabet headers

template <bio::alphabet::alphabet t>
void print(t const v)
{
    std::cout << "I am an alphabet and my value as char is: " << bio::alphabet::to_char(v) << '\n';
}

using namespace bio::alphabet::literals;

int main()
{
    auto d = 'A'_dna5;
    auto a = 'L'_aa27;
    auto g = bio::alphabet::gap{};

    print(d);
    print(a);
    print(g);
}
