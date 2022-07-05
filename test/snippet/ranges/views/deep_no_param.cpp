#include <vector>

#include <bio/alphabet/nucleotide/dna5.hpp>
#include <bio/ranges/views/deep.hpp>
#include <ranges>

namespace my
{
// You can create a permanent alias:
inline auto const deep_reverse = bio::views::deep{std::views::reverse};
}

int main()
{
    using bio::alphabet::operator""_dna5;

    std::vector<bio::alphabet::dna5_vector> foo{"AAATTT"_dna5, "CCCGGG"_dna5};

    auto r = foo | std::views::reverse;                     // == [ [C,C,C,G,G,G], [A,A,A,T,T,T] ]

    // These two are equivalent:
    auto e = foo | my::deep_reverse;                       // == [ [T,T,T,A,A,A], [G,G,G,C,C,C] ]
    auto d = foo | bio::views::deep{std::views::reverse}; // == [ [T,T,T,A,A,A], [G,G,G,C,C,C] ]
}
