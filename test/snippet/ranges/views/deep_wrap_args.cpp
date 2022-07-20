#include <vector>

#include <bio/alphabet/nucleotide/dna5.hpp>
#include <bio/ranges/views/deep.hpp>
#include <ranges>

namespace my
{
inline auto const deep_take1 = bio::views::deep{std::views::take(1)};
}

int main()
{
    using namespace bio::alphabet::literals;

    std::vector<bio::alphabet::dna5_vector> foo{"AAATTT"_dna5, "CCCGGG"_dna5};

    auto t = foo | std::views::take(1);                      // == [ [A,A,A,T,T,T] ]

    auto d = foo | bio::views::deep{std::views::take(1)}; // == [ [A], [C] ]
    // constructor arguments passed via {} and arguments to underlying view hardcoded inside

    // or with an alias defined previously
    auto e = foo | my::deep_take1;                           // == [ [A], [C] ]
}
