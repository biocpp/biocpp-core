#include <bio/alphabet/quality/qualified.hpp>
#include <bio/alphabet/quality/phred42.hpp>
#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/alphabet/fmt.hpp>

int main()
{
    using bio::operator""_dna4;
    using bio::get;

    bio::qualified<bio::dna4, bio::phred42> letter{'A'_dna4, bio::phred42{7}};
    fmt::print("{} {} {}\n",
               bio::to_rank(letter),
               bio::to_rank(get<0>(letter)),
               bio::to_rank(get<1>(letter)));
    // 28 0 7

    fmt::print("{} {} {}\n",
               bio::to_char(letter),
               bio::to_char(get<0>(letter)),
               bio::to_char(get<1>(letter)));
    // A A (

    fmt::print("{} {}\n", bio::to_phred(letter), bio::to_phred(get<1>(letter)));
    // 7 7

    // modify via structured bindings and references:
    auto & [ seq_l, qual_l ] = letter;
    seq_l = 'G'_dna4;
    fmt::print("{}\n", bio::to_char(letter));
    // G
}
