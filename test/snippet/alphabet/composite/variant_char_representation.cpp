#include <bio/alphabet/composite/variant.hpp>
#include <bio/alphabet/nucleotide/all.hpp>

int main()
{
    using namespace bio::alphabet::literals;

    bio::alphabet::variant<bio::alphabet::dna4, bio::alphabet::dna5> var;
    var.assign_char('A');             // will be in the "dna4-state"
    var = 'A'_dna5;                   // will be in the "dna5-state"
}
