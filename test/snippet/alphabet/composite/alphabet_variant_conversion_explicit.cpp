#include <bio/alphabet/composite/alphabet_variant.hpp>
#include <bio/alphabet/gap/gap.hpp>
#include <bio/alphabet/nucleotide/all.hpp>

int main()
{
    using seqan3::operator""_dna5;

    // possible:
    seqan3::alphabet_variant<seqan3::dna4, seqan3::gap> letter1{'C'_dna5};
    // not possible:
    // seqan3::alphabet_variant<seqan3::dna4, seqan3::gap> letter2 = 'C'_dna5;
}
