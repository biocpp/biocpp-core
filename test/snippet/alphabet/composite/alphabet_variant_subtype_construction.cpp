#include <bio/alphabet/composite/alphabet_variant.hpp>
#include <bio/alphabet/gap/gap.hpp>
#include <bio/alphabet/nucleotide/all.hpp>

int main()
{
    using namespace bio::alphabet::literals;

    bio::alphabet::alphabet_variant<bio::alphabet::dna4, bio::alphabet::gap> letter1{};
    letter1 = 'C'_rna4;
}
