#include <bio/alphabet/composite/alphabet_variant.hpp>
#include <bio/alphabet/nucleotide/all.hpp>

int main()
{
    using bio::operator""_dna5;

    bio::alphabet_variant<bio::dna4, bio::dna5> var;
    var.assign_char('A');             // will be in the "dna4-state"
    var = 'A'_dna5;                   // will be in the "dna5-state"
}
