#include <bio/alphabet/concept.hpp>
#include <bio/alphabet/adaptation/char.hpp>
#include <bio/alphabet/nucleotide/dna5.hpp>

using bio::alphabet::operator""_dna5;

int main()
{
    auto r2 = bio::alphabet::to_char('A');         // calls bio::alphabet::custom::to_char('A'); r2 == 'A'
    auto r3 = bio::alphabet::to_char('A'_dna5);    // calls .to_char() member; r3 == 'A'
}
