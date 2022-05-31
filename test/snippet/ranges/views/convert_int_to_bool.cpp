#include <vector>

#include <bio/alphabet/nucleotide/dna15.hpp>
#include <bio/alphabet/nucleotide/dna5.hpp>
#include <bio/ranges/views/convert.hpp>
#include <bio/ranges/views/to.hpp>
#include <ranges>

int main()
{
    // convert from int to bool
    std::vector<int>  vec{7, 5, 0, 5, 0, 0, 4, 8, -3};

    // pipe notation
    auto v = vec | seqan3::views::convert<bool>; // == [1, 1, 0, 1, 0, 0, 1, 1, 1];

    // function notation and immediate conversion to vector again
    auto v2 = seqan3::views::convert<bool>(vec) | seqan3::views::to<std::vector<bool>>();

    // combinability
    auto v3 = vec | seqan3::views::convert<bool> | std::views::reverse; // == [1, 1, 1, 0, 0, 1, 0, 1, 1];
}
