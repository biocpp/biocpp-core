#include <iostream>

#include <bio/alphabet/nucleotide/dna5.hpp>
#include <bio/meta/debug_stream.hpp>
#include <bio/ranges/views/to_rank.hpp>

int main()
{
    using bio::operator""_dna5;

    // The alphabet normally needs to be converted to char explicitly:
    std::cout << bio::to_char('C'_dna5);                // prints 'C'

    // The debug_stream, on the other hand, does this automatically:
    bio::debug_stream << 'C'_dna5;                      // prints 'C'

    // The debug_stream can also print all types that model std::ranges::input_range:
    std::vector<bio::dna5> vec{"ACGT"_dna5};
    bio::debug_stream << vec;                           // prints "ACGT"

    // ranges of non-alphabets are printed comma-separated:
    bio::debug_stream << (vec | bio::views::to_rank); // prints "[0,1,2,3]"
}
