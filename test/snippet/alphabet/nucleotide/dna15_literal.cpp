#include <bio/alphabet/nucleotide/dna15.hpp>

int main()
{
    using namespace bio::alphabet::literals;

    bio::alphabet::dna15_vector foo{"ACGTTA"_dna15};
    bio::alphabet::dna15_vector bar = "ACGTTA"_dna15;
    auto bax = "ACGTTA"_dna15;
}
