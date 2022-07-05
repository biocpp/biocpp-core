#include <vector>

#include <bio/alphabet/nucleotide/dna5.hpp>
#include <bio/ranges/views/deep.hpp>
#include <bio/ranges/views/take.hpp>

namespace my
{
inline auto const deep_take = bio::views::deep{std::views::take};
}

int main()
{
    using bio::alphabet::operator""_dna5;

    std::vector<bio::alphabet::dna5_vector> foo{"AAATTT"_dna5, "CCCGGG"_dna5};

    auto t = foo | std::views::take(1);                   // == [ [A,A,A,T,T,T] ]

    auto d = foo | bio::views::deep{std::views::take}(1); // == [ [A], [C] ]
    // constructor arguments passed via {} and arguments to underlying view passed via ()

    // In this case especially, an alias improves readability:
    auto e = foo | my::deep_take(1);                         // == [ [A], [C] ]
}
