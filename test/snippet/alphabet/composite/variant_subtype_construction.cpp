#include <bio/alphabet/composite/variant.hpp>
#include <bio/alphabet/gap/gap.hpp>
#include <bio/alphabet/nucleotide/all.hpp>

int main()
{
    using namespace bio::alphabet::literals;

    bio::alphabet::variant<bio::alphabet::dna4, bio::alphabet::gap> letter1{};
    letter1 = 'C'_rna4;
}
