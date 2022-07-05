#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/alphabet/quality/phred42.hpp>
#include <bio/alphabet/quality/qualified.hpp>
#include <bio/alphabet/fmt.hpp>

int main()
{
    using bio::alphabet::operator""_dna4;

    bio::alphabet::qualified<bio::alphabet::dna4, bio::alphabet::phred42> letter1{'C'_dna4};
    // creates {'C'_dna4, bio::alphabet::phred42{0}}
    bio::alphabet::qualified<bio::alphabet::dna4, bio::alphabet::phred42> letter2{bio::alphabet::phred42{1}};
    // creates {'A'_dna4, bio::alphabet::phred42{1}}

    if (letter1 == letter2)
        fmt::print("{}", "yeah\n"); // yeah
}
