#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/ranges/views/complement.hpp>
#include <bio/ranges/to.hpp>
#include <ranges>

int main()
{
    using namespace bio::alphabet::literals;

    std::vector<bio::alphabet::dna4> vec{"ACGGTC"_dna4};
    auto vec_view2 = bio::views::complement(vec);

    // re-convert to container
    std::vector<bio::alphabet::dna4> complemented = vec_view2 | bio::ranges::to<std::vector<bio::alphabet::dna4>>();
    assert(complemented == "TGCCAG"_dna4);

    // also possible in one step
    std::vector<bio::alphabet::dna4> reversed = vec | std::views::reverse | bio::ranges::to<std::vector<bio::alphabet::dna4>>();
    assert(reversed == "CTGGCA"_dna4);
}
