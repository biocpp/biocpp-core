//![start]
#include <ranges>
#include <bio/alphabet/nucleotide/all.hpp>
#include <bio/alphabet/fmt.hpp>

using bio::alphabet::operator""_dna5;

//![start]
auto my_reverse_complement = std::views::reverse | std::views::transform(bio::alphabet::complement);

//![end]
int main()
{
    std::vector<bio::alphabet::dna5> vec{"ACCAGATTA"_dna5};
    fmt::print("{}\n", vec);                     // prints "ACCAGATTA"

    auto v = vec | my_reverse_complement;
    fmt::print("{}\n", v);                       // prints "TAATCTGGT"
}
//![end]
