#include <bio/alphabet/nucleotide/all.hpp>
#include <bio/alphabet/quality/phred42.hpp>
#include <bio/alphabet/quality/qualified.hpp>

int main()
{
    using namespace bio::alphabet::literals;
    using namespace bio::alphabet::literals;

    bio::alphabet::qualified<bio::alphabet::dna4, bio::alphabet::phred42> letter1{'T'_dna4, bio::alphabet::phred42{1}};

    letter1 = 'C'_rna4; // yields {'C'_dna4, bio::alphabet::phred42{1}}
}
