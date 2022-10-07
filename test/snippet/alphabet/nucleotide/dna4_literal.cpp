#include <bio/alphabet/nucleotide/dna4.hpp>

int main()
{
    using namespace bio::alphabet::literals;

    std::vector<bio::alphabet::dna4> foo{"ACGTTA"_dna4};
    std::vector<bio::alphabet::dna4> bar = "ACGTTA"_dna4;
    auto bax = "ACGTTA"_dna4;
}
