#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/alphabet/quality/phred42.hpp>
#include <bio/alphabet/quality/qualified.hpp>
#include <bio/meta/debug_stream.hpp>

int main()
{
    using bio::operator""_dna4;

    bio::qualified<bio::dna4, bio::phred42> letter1{'C'_dna4};
    // creates {'C'_dna4, bio::phred42{0}}
    bio::qualified<bio::dna4, bio::phred42> letter2{bio::phred42{1}};
    // creates {'A'_dna4, bio::phred42{1}}

    if (letter1 == letter2)
        bio::debug_stream << "yeah\n"; // yeah
}
