#include <bio/alphabet/composite/alphabet_variant.hpp>
#include <bio/alphabet/gap/gap.hpp>
#include <bio/alphabet/nucleotide/all.hpp>

int main()
{
    using bio::operator""_rna4;

    bio::alphabet_variant<bio::dna4, bio::gap> letter1{};
    letter1 = 'C'_rna4;
}
