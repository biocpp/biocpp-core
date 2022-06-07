#include <bio/alphabet/nucleotide/all.hpp>
#include <bio/alphabet/quality/phred42.hpp>
#include <bio/alphabet/quality/qualified.hpp>
#include <bio/alphabet/fmt.hpp>

int main()
{
    using bio::operator""_dna4;
    using bio::operator""_rna4;

    // The following creates {'C'_dna4, phred42{0}}
    bio::qualified<bio::dna4, bio::phred42> letter1{'C'_dna4};
    // The following also creates {'C'_dna4, bio::phred42{0}}, since rna4 assignable to dna4
    bio::qualified<bio::dna4, bio::phred42> letter2{'C'_rna4};

    if (letter1 == letter2)
        fmt::print("{}", "yeah\n"); // yeah
}
