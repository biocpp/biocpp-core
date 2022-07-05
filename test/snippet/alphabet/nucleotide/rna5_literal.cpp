#include <bio/alphabet/nucleotide/rna5.hpp>

int main()
{
    using bio::alphabet::operator""_rna5;

    bio::alphabet::rna5_vector foo{"ACGTTA"_rna5};
    bio::alphabet::rna5_vector bar = "ACGTTA"_rna5;
    auto bax = "ACGTTA"_rna5;
}
