#include <bio/alphabet/concept.hpp>
#include <bio/alphabet/adaptation/char.hpp>
#include <bio/alphabet/nucleotide/dna5.hpp>

using bio::operator""_dna5;

int main()
{
    auto r2 = bio::to_char('A');         // calls bio::custom::to_char('A'); r2 == 'A'
    auto r3 = bio::to_char('A'_dna5);    // calls .to_char() member; r3 == 'A'
}
