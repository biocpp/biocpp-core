#include <bio/alphabet/nucleotide/dna5.hpp>
#include <bio/ranges/views/add_reverse_complement.hpp>
#include <bio/ranges/views/complement.hpp>
#include <ranges>
#include <vector>

int main()
{
    using namespace bio::alphabet::literals;

    std::vector<std::vector<bio::alphabet::dna5>> vec{{"ACGTACGTACGTA"_dna5},
                                                      {"TCGAGAGCTTTAGC"_dna5}};

    // pipe notation
    auto v1 = vec | bio::views::add_reverse_complement;
    // == [ACGTACGTACGTA, TACGTACGTACGT, TCGAGAGCTTTAGC, GCTAAAGCTCTCGA]

    // function notation
    auto v2(bio::views::add_reverse_complement(vec));
    // == [ACGTACGTACGTA, TACGTACGTACGT, TCGAGAGCTTTAGC, GCTAAAGCTCTCGA]

    // combinability
    auto v3 = vec | bio::views::complement | bio::views::add_reverse_complement;
    // == [TGCATGCATGCAT, ATGCATGCATGCA, AGCTCTCGAAATCG, CGATTTCGAGAGCT]
}
