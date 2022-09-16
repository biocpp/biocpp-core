#include <bio/alphabet/composite/variant.hpp>
#include <bio/alphabet/gap/gap.hpp>
#include <bio/alphabet/nucleotide/all.hpp>

int main()
{
    using namespace bio::alphabet::literals;

    // possible:
    bio::alphabet::variant<bio::alphabet::dna4, bio::alphabet::gap> letter1{'C'_dna5};
    // not possible:
    // bio::alphabet::variant<bio::alphabet::dna4, bio::alphabet::gap> letter2 = 'C'_dna5;
}
