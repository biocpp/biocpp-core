#include <bio/alphabet/nucleotide/dna15.hpp>

int main()
{
    using namespace bio::alphabet::literals;

    std::vector<bio::alphabet::dna15> foo{"ACGTTA"_dna15};
    std::vector<bio::alphabet::dna15> bar = "ACGTTA"_dna15;
    auto bax = "ACGTTA"_dna15;
}
