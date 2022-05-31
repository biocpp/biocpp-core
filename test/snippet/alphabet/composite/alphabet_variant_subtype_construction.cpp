#include <bio/alphabet/composite/alphabet_variant.hpp>
#include <bio/alphabet/gap/gap.hpp>
#include <bio/alphabet/nucleotide/all.hpp>

int main()
{
    using seqan3::operator""_rna4;

    seqan3::alphabet_variant<seqan3::dna4, seqan3::gap> letter1{};
    letter1 = 'C'_rna4;
}
