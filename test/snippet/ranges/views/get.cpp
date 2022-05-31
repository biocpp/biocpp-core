#include <vector>

#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/alphabet/quality/aliases.hpp> // includes seqan3::dna4q
#include <bio/alphabet/quality/phred42.hpp>
#include <bio/meta/debug_stream.hpp>
#include <bio/ranges/views/get.hpp>
#include <bio/ranges/views/to_char.hpp>

int main()
{
    using seqan3::operator""_dna4;
    using seqan3::operator""_phred42;

    // Create a vector of dna4 quality composite alphabet.
    std::vector<seqan3::dna4q> qv{{'A'_dna4, '0'_phred42},
                                  {'C'_dna4, '1'_phred42},
                                  {'G'_dna4, '2'_phred42},
                                  {'T'_dna4, '3'_phred42}};

    seqan3::debug_stream << (qv | seqan3::views::get<0> | seqan3::views::to_char) << '\n'; // Prints [A,C,G,T]
    seqan3::debug_stream << (qv | seqan3::views::get<1> | seqan3::views::to_char) << '\n'; // Prints [!,",#,$]
}
