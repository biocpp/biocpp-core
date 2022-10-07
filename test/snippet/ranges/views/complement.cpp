#include <bio/alphabet/nucleotide/dna5.hpp>
#include <bio/ranges/views/complement.hpp>
#include <ranges>

int main()
{
    using namespace bio::alphabet::literals;

    std::vector<bio::alphabet::dna5> foo{"ACGTA"_dna5};

    // pipe notation
    auto v = foo | bio::views::complement;                        // == "TGCAT"

    // function notation
    auto v2(bio::views::complement(foo));                         // == "TGCAT"

    // generate the reverse complement:
    auto v3 = foo | bio::views::complement | std::views::reverse; // == "TACGT"
}
