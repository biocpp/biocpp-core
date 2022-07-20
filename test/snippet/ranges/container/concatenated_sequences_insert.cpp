#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/alphabet/fmt.hpp>
#include <bio/ranges/container/concatenated_sequences.hpp>

int main()
{
    using namespace bio::alphabet::literals;

    bio::ranges::concatenated_sequences<bio::alphabet::dna4_vector> foobar;
    foobar.insert(foobar.end(), "ACGT"_dna4);
    fmt::print("{}\n", foobar[0]); // "ACGT"
}
