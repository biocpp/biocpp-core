#include <bio/alphabet/nucleotide/dna15.hpp>

int main()
{
    using bio::alphabet::operator""_dna15;

    bio::alphabet::dna15_vector foo{"ACGTTA"_dna15};
    bio::alphabet::dna15_vector bar = "ACGTTA"_dna15;
    auto bax = "ACGTTA"_dna15;
}
