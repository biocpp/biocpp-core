#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/alphabet/fmt.hpp>
#include <bio/ranges/container/concatenated_sequences.hpp>

int main()
{
    using namespace bio::alphabet::literals;

    bio::ranges::concatenated_sequences<std::vector<bio::alphabet::dna4>> foobar;
    foobar.insert(foobar.end(), 2, "ACGT"_dna4);
    fmt::print("{}\n", foobar[0]); // "ACGT"
    fmt::print("{}\n", foobar[1]); // "ACGT"
}
