#include <bio/alphabet/nucleotide/rna5.hpp>

int main()
{
    using bio::operator""_rna5;
    
    bio::rna5_vector foo{"ACGTTA"_rna5};
    bio::rna5_vector bar = "ACGTTA"_rna5;
    auto bax = "ACGTTA"_rna5;
}
