#include <vector>

#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/alphabet/nucleotide/dna5.hpp>
#include <bio/ranges/views/rank_to.hpp>

int main()
{
    std::vector<int> vec{0, 1, 3, 3, 3, 2, 0, 3, 0};
    auto v1 = vec | seqan3::views::rank_to<seqan3::dna4>; // == "ACTTTGATA"_dna4
    auto v2 = vec | seqan3::views::rank_to<seqan3::dna5>; // == "ACTTTGATA"_dna5
}
