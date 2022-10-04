#include <bio/alphabet/concept.hpp>
#include <bio/alphabet/custom/char.hpp>
#include <bio/alphabet/nucleotide/dna5.hpp>

using namespace bio::alphabet::literals;

int main()
{
    auto r2 = bio::alphabet::to_char('A');         // calls bio::alphabet::custom::tag_invoke(bio::alphabet::custom::to_char, 'A'); r2 == 'A'
    auto r3 = bio::alphabet::to_char('A'_dna5);    // calls .to_char() member; r3 == 'A'
}
