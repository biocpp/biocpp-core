#include <bio/alphabet/quality/qualified.hpp>
#include <bio/alphabet/quality/phred42.hpp>
#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/alphabet/fmt.hpp>

int main()
{
    using namespace bio::alphabet::literals;

    bio::alphabet::qualified<bio::alphabet::dna4, bio::alphabet::phred42> letter{'A'_dna4, bio::alphabet::phred42{7}};
    fmt::print("{} {} {}\n",
               bio::alphabet::to_rank(letter),
               bio::alphabet::to_rank(get<0>(letter)),
               bio::alphabet::to_rank(get<1>(letter)));
    // 28 0 7

    fmt::print("{} {} {}\n",
               bio::alphabet::to_char(letter),
               bio::alphabet::to_char(get<0>(letter)),
               bio::alphabet::to_char(get<1>(letter)));
    // A A (

    fmt::print("{} {}\n", bio::alphabet::to_phred(letter), bio::alphabet::to_phred(get<1>(letter)));
    // 7 7

    // modify via structured bindings and references:
    auto & [ seq_l, qual_l ] = letter;
    seq_l = 'G'_dna4;
    fmt::print("{}\n", bio::alphabet::to_char(letter));
    // G
}
