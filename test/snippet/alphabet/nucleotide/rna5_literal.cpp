#include <bio/alphabet/nucleotide/rna5.hpp>

int main()
{
    using namespace bio::alphabet::literals;

    std::vector<bio::alphabet::rna5> foo{"ACGTTA"_rna5};
    std::vector<bio::alphabet::rna5> bar = "ACGTTA"_rna5;
    auto bax = "ACGTTA"_rna5;
}
