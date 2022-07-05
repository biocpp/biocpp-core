#include <bio/alphabet/nucleotide/concept.hpp>
#include <bio/alphabet/nucleotide/rna5.hpp>

using bio::alphabet::operator""_rna5;

int main()
{
    auto r1 = 'A'_rna5.complement();        // calls member function rna5::complement(); r1 == 'U'_rna5
    auto r2 = bio::alphabet::complement('A'_rna5); // calls global complement() function on the rna5 object; r2 == 'U'_rna5
}
