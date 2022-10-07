#include <bio/alphabet/nucleotide/rna4.hpp>

int main()
{
    using namespace bio::alphabet::literals;

    std::vector<bio::alphabet::rna4> foo{"ACGTTA"_rna4};
    std::vector<bio::alphabet::rna4> bar = "ACGTTA"_rna4;
    auto bax = "ACGTTA"_rna4;
}
