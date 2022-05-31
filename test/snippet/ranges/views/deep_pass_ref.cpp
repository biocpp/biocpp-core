#include <vector>

#include <bio/alphabet/nucleotide/dna5.hpp>
#include <bio/ranges/views/deep.hpp>
#include <bio/ranges/views/take.hpp>

namespace my
{
inline auto const deep_take = seqan3::views::deep{std::views::take};
}

int main()
{
    using seqan3::operator""_dna5;

    std::vector<seqan3::dna5_vector> foo{"AAATTT"_dna5, "CCCGGG"_dna5};

    int i = 3;
    auto f = foo | my::deep_take(i); // takes `i` as a reference!
}
