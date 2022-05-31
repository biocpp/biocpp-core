#include <bio/alphabet/composite/alphabet_variant.hpp>
#include <bio/alphabet/gap/gap.hpp>
#include <bio/alphabet/nucleotide/all.hpp>

int main()
{
    using bio::operator""_dna4;

    bio::alphabet_variant<bio::dna4, bio::gap> letter1{'C'_dna4}; // or
    bio::alphabet_variant<bio::dna4, bio::gap> letter2 = bio::gap{};
}
