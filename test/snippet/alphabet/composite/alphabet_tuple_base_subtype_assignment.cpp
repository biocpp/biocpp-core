#include <bio/alphabet/nucleotide/all.hpp>
#include <bio/alphabet/quality/phred42.hpp>
#include <bio/alphabet/quality/qualified.hpp>

int main()
{
    using bio::operator""_dna4;
    using bio::operator""_rna4;

    bio::qualified<bio::dna4, bio::phred42> letter1{'T'_dna4, bio::phred42{1}};

    letter1 = 'C'_rna4; // yields {'C'_dna4, bio::phred42{1}}
}
