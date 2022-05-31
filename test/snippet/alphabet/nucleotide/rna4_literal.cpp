#include <bio/alphabet/nucleotide/rna4.hpp>

int main()
{
    using bio::operator""_rna4;
    
    bio::rna4_vector foo{"ACGTTA"_rna4};
    bio::rna4_vector bar = "ACGTTA"_rna4;
    auto bax = "ACGTTA"_rna4;
}
