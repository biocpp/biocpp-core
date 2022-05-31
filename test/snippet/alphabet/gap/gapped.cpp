#include <bio/alphabet/gap/gapped.hpp>
#include <bio/alphabet/nucleotide/dna4.hpp>

int main()
{
    using bio::operator""_dna4;

    bio::gapped<bio::dna4> gapped_letter{};
    bio::gapped<bio::dna4> converted_letter{'C'_dna4};
    bio::gapped<bio::dna4> gap_letter{bio::gap{}};

    bio::gapped<bio::dna4>{}.assign_char('C');
    bio::gapped<bio::dna4>{}.assign_char('-'); // gap character
    bio::gapped<bio::dna4>{}.assign_char('K'); // unknown characters map to the default/unknown
                                                     // character of the given alphabet type (i.e. A of dna4)
}
