#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/ranges/views/complement.hpp>
#include <ranges>

int main()
{
    using namespace bio::alphabet::literals;

    bio::alphabet::dna4_vector vec{"ACGGTC"_dna4};
    // views can be composed iteratively
    auto vec_view3 = vec | std::views::reverse;
    auto vec_view4 = vec_view3 | bio::views::complement;

    // or in one line similar to the unix command line
    auto vec_view5 = vec | bio::views::complement | std::views::reverse;

    // vec_view4 and vec_view5 are the reverse complement of "ACGGTC": "GACCGT"
}
