#include <iostream>                     // for std::cout
#include <bio/alphabet/all.hpp>      // include all alphabet headers

template <bio::alphabet::alphabet t>
void print(t const v)
{
    std::cout << "I am an alphabet and my value as char is: " << bio::alphabet::to_char(v) << '\n';
}

template <bio::alphabet::nucleotide_alphabet t>
void print(t const v)
{
    std::cout << "I am a nucleotide, my value as char is: " << bio::alphabet::to_char(v)
              << " and my complement is: " << bio::alphabet::to_char(bio::alphabet::complement(v)) << '\n';
}

using bio::alphabet::operator""_dna5;
using bio::alphabet::operator""_aa27;

int main()
{
    auto d = 'A'_dna5;
    auto a = 'L'_aa27;
    auto g = bio::alphabet::gap{};

    print(d);
    print(a);
    print(g);
}
