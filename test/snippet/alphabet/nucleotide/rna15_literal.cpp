#include <bio/alphabet/nucleotide/rna15.hpp>

int main()
{
    using namespace bio::alphabet::literals;

    std::vector<bio::alphabet::rna15> foo{"ACGTTA"_rna15};
    std::vector<bio::alphabet::rna15> bar = "ACGTTA"_rna15;
    auto bax = "ACGTTA"_rna15;
}
