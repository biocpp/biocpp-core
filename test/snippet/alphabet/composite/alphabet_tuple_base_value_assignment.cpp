#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/alphabet/quality/phred42.hpp>
#include <bio/alphabet/quality/qualified.hpp>

int main()
{
    using bio::operator""_dna4;

    bio::qualified<bio::dna4, bio::phred42> letter1{'T'_dna4, bio::phred42{1}};

    letter1 = 'C'_dna4;           // yields {'C'_dna4, bio::phred42{1}}
    letter1 = bio::phred42{2}; // yields {'C'_dna4, bio::phred42{2}}
}
