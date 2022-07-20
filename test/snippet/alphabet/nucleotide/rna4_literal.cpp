#include <bio/alphabet/nucleotide/rna4.hpp>

int main()
{
    using namespace bio::alphabet::literals;

    bio::alphabet::rna4_vector foo{"ACGTTA"_rna4};
    bio::alphabet::rna4_vector bar = "ACGTTA"_rna4;
    auto bax = "ACGTTA"_rna4;
}
