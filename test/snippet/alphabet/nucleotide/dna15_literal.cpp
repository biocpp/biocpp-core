#include <bio/alphabet/nucleotide/dna15.hpp>

int main()
{
    using bio::operator""_dna15;
    
    bio::dna15_vector foo{"ACGTTA"_dna15};
    bio::dna15_vector bar = "ACGTTA"_dna15;
    auto bax = "ACGTTA"_dna15;
}
