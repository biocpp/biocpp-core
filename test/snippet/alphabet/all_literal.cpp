#include <bio/alphabet/nucleotide/dna4.hpp>

int main()
{
    using bio::operator""_dna4;
    bio::dna4        my_letter = 'A'_dna4;           // identical to assign_char_to('A', my_letter);
    bio::dna4_vector my_seq    = "ACGT"_dna4;        // identical to calling assign_char for each element
}
