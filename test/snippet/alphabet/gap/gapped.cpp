#include <bio/alphabet/gap/gapped.hpp>
#include <bio/alphabet/nucleotide/dna4.hpp>

int main()
{
    using bio::alphabet::operator""_dna4;

    bio::alphabet::gapped<bio::alphabet::dna4> gapped_letter{};
    bio::alphabet::gapped<bio::alphabet::dna4> converted_letter{'C'_dna4};
    bio::alphabet::gapped<bio::alphabet::dna4> gap_letter{bio::alphabet::gap{}};

    bio::alphabet::gapped<bio::alphabet::dna4>{}.assign_char('C');
    bio::alphabet::gapped<bio::alphabet::dna4>{}.assign_char('-'); // gap character
    bio::alphabet::gapped<bio::alphabet::dna4>{}.assign_char('K'); // unknown characters map to the default/unknown
                                                     // character of the given alphabet type (i.e. A of dna4)
}
