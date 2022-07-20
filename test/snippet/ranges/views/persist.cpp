#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/ranges/views/persist.hpp>
#include <bio/ranges/views/to_char.hpp>

int main()
{
    using namespace bio::alphabet::literals;

    // explicitly create an l-value of our dna vector:
    auto vec = "ACGT"_dna4;
    auto v = vec | bio::views::to_char;

    // using bio::views::persist you can bind the temporary directly:
    auto v2 = "ACGT"_dna4 | bio::views::persist | bio::views::to_char;

    // note that bio::views::persist must follow immediately after the temporary,
    // thus the function notation might be more intuitive:
    auto v3 = bio::views::persist("ACGT"_dna4) | bio::views::to_char;
}
