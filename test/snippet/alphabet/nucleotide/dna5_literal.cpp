#include <bio/alphabet/nucleotide/dna5.hpp>

int main()
{
    using bio::alphabet::operator""_dna5;

    bio::alphabet::dna5_vector foo{"ACGTTA"_dna5};
    bio::alphabet::dna5_vector bar = "ACGTTA"_dna5;
    auto bax = "ACGTTA"_dna5;
}
