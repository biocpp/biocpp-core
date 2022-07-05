#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/alphabet/quality/phred42.hpp>
#include <bio/alphabet/quality/qualified.hpp>

int main()
{
    using bio::alphabet::operator""_dna4;

    bio::alphabet::qualified<bio::alphabet::dna4, bio::alphabet::phred42> letter1{'T'_dna4, bio::alphabet::phred42{1}};

    letter1 = 'C'_dna4;           // yields {'C'_dna4, bio::alphabet::phred42{1}}
    letter1 = bio::alphabet::phred42{2}; // yields {'C'_dna4, bio::alphabet::phred42{2}}
}
