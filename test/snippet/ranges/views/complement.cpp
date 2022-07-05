#include <bio/alphabet/nucleotide/dna5.hpp>
#include <bio/ranges/views/complement.hpp>
#include <ranges>

int main()
{
    using bio::alphabet::operator""_dna5;

    bio::alphabet::dna5_vector foo{"ACGTA"_dna5};

    // pipe notation
    auto v = foo | bio::views::complement;                        // == "TGCAT"

    // function notation
    auto v2(bio::views::complement(foo));                         // == "TGCAT"

    // generate the reverse complement:
    auto v3 = foo | bio::views::complement | std::views::reverse; // == "TACGT"
}
