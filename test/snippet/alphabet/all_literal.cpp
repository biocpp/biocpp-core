#include <bio/alphabet/nucleotide/dna4.hpp>

int main()
{
    using namespace bio::alphabet::literals;
    bio::alphabet::dna4        my_letter = 'A'_dna4;           // identical to assign_char_to('A', my_letter);
    bio::alphabet::dna4_vector my_seq    = "ACGT"_dna4;        // identical to calling assign_char for each element
}
