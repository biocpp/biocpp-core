#include <bio/alphabet/concept.hpp>
#include <bio/alphabet/adaptation/char.hpp>
#include <bio/alphabet/nucleotide/dna5.hpp>

using namespace bio::alphabet::literals;

int main()
{
    auto r2 = bio::alphabet::to_rank('A');         // calls bio::alphabet::cpo::tag_invoke(bio::alphabet::cpo::to_rank, 'A'); r2 == 65
    auto r3 = bio::alphabet::to_rank('A'_dna5);    // calls .to_char() member; r3 == 0
}
