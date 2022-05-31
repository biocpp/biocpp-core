#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/ranges/views/complement.hpp>
#include <bio/ranges/views/to.hpp>
#include <ranges>

int main()
{
    using seqan3::operator""_dna4;

    seqan3::dna4_vector vec{"ACGGTC"_dna4};
    auto vec_view2 = seqan3::views::complement(vec);

    // re-convert to container
    seqan3::dna4_vector complemented = vec_view2 | seqan3::views::to<seqan3::dna4_vector>();
    assert(complemented == "TGCCAG"_dna4);

    // also possible in one step
    seqan3::dna4_vector reversed = vec | std::views::reverse | seqan3::views::to<seqan3::dna4_vector>();
    assert(reversed == "CTGGCA"_dna4);
}
