#include <iostream>

#include <bio/alphabet/fmt.hpp>
#include <bio/alphabet/nucleotide/all.hpp>
#include <bio/ranges/views/transform_by_pos.hpp>

using namespace bio::alphabet::literals;

int main()
{
    std::vector vec = "ACGT"_dna5;

    auto rev = [] (auto && urange, size_t const pos) -> std::ranges::range_reference_t<decltype(urange)>
    {
        return urange[std::ranges::size(urange) - 1 - pos];
    };

    auto v = vec | bio::views::transform_by_pos(rev);

    fmt::print("{}\n", v); // prints: TGCA

    return 0;
}
