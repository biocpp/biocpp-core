#include <bio/alphabet/nucleotide/rna15.hpp>

int main()
{
    using bio::alphabet::operator""_rna15;

    bio::alphabet::rna15_vector foo{"ACGTTA"_rna15};
    bio::alphabet::rna15_vector bar = "ACGTTA"_rna15;
    auto bax = "ACGTTA"_rna15;
}
