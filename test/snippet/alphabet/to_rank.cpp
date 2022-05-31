#include <bio/alphabet/concept.hpp>
#include <bio/alphabet/adaptation/char.hpp>
#include <bio/alphabet/nucleotide/dna5.hpp>

using bio::operator""_dna5;

int main()
{
    auto r2 = bio::to_rank('A');         // calls bio::custom::to_rank('A'); r2 == 65
    auto r3 = bio::to_rank('A'_dna5);    // calls .to_char() member; r3 == 0
}
