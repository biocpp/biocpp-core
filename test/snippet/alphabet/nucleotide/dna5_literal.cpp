#include <bio/alphabet/nucleotide/dna5.hpp>

int main()
{
    using namespace bio::alphabet::literals;

    std::vector<bio::alphabet::dna5> foo{"ACGTTA"_dna5};
    std::vector<bio::alphabet::dna5> bar = "ACGTTA"_dna5;
    auto bax = "ACGTTA"_dna5;
}
