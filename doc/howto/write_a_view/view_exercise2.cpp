//![start]
#include <ranges>
#include <bio/alphabet/nucleotide/all.hpp>
#include <bio/meta/debug_stream.hpp>

using bio::operator""_dna5;

//![start]
auto my_reverse_complement = std::views::reverse | std::views::transform([] (auto const d)
{
    return bio::complement(d);
});

//![end]
int main()
{
    std::vector<bio::dna5> vec{"ACCAGATTA"_dna5};
    bio::debug_stream << vec << '\n';            // will print "ACCAGATTA"

    auto v = vec | my_reverse_complement;

    bio::debug_stream << v << '\n';              // prints "TAATCTGGT"
}
//![end]
