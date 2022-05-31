#include <bio/alphabet/composite/alphabet_variant.hpp>
#include <bio/alphabet/gap/gap.hpp>
#include <bio/alphabet/nucleotide/all.hpp>

int main()
{
    using bio::operator""_dna5;

    // possible:
    bio::alphabet_variant<bio::dna4, bio::gap> letter1{'C'_dna5};
    // not possible:
    // bio::alphabet_variant<bio::dna4, bio::gap> letter2 = 'C'_dna5;
}
