#include <bio/alphabet/nucleotide/dna4.hpp>

int main()
{
    using bio::operator""_dna4;
    
    bio::dna4_vector foo{"ACGTTA"_dna4};
    bio::dna4_vector bar = "ACGTTA"_dna4;
    auto bax = "ACGTTA"_dna4;
}
