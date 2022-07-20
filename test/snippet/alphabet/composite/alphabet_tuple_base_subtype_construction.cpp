#include <bio/alphabet/nucleotide/all.hpp>
#include <bio/alphabet/quality/phred42.hpp>
#include <bio/alphabet/quality/qualified.hpp>
#include <bio/alphabet/fmt.hpp>

int main()
{
    using namespace bio::alphabet::literals;
    using namespace bio::alphabet::literals;

    // The following creates {'C'_dna4, phred42{0}}
    bio::alphabet::qualified<bio::alphabet::dna4, bio::alphabet::phred42> letter1{'C'_dna4};
    // The following also creates {'C'_dna4, bio::alphabet::phred42{0}}, since rna4 assignable to dna4
    bio::alphabet::qualified<bio::alphabet::dna4, bio::alphabet::phred42> letter2{'C'_rna4};

    if (letter1 == letter2)
        fmt::print("{}", "yeah\n"); // yeah
}
