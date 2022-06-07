#include <vector>

#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/alphabet/quality/aliases.hpp> // includes bio::dna4q
#include <bio/alphabet/quality/phred42.hpp>
#include <bio/alphabet/fmt.hpp>
#include <bio/ranges/views/get.hpp>
#include <bio/ranges/views/to_char.hpp>

int main()
{
    using bio::operator""_dna4;
    using bio::operator""_phred42;

    // Create a vector of dna4 quality composite alphabet.
    std::vector<bio::dna4q> qv{{'A'_dna4, '0'_phred42},
                               {'C'_dna4, '1'_phred42},
                               {'G'_dna4, '2'_phred42},
                               {'T'_dna4, '3'_phred42}};

    fmt::print("{}\n", qv | bio::views::get<0> | bio::views::to_char); // Prints [A,C,G,T]
    fmt::print("{}\n", qv | bio::views::get<1> | bio::views::to_char); // Prints [!,",#,$]
}
