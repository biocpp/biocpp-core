#include <bio/alphabet/composite/alphabet_variant.hpp>
#include <bio/alphabet/nucleotide/all.hpp>

int main()
{
    using bio::alphabet::operator""_dna5;

    bio::alphabet::alphabet_variant<bio::alphabet::dna4, bio::alphabet::dna5> var;
    var.assign_char('A');             // will be in the "dna4-state"
    var = 'A'_dna5;                   // will be in the "dna5-state"
}
