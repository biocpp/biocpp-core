#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/alphabet/fmt.hpp>
#include <bio/ranges/container/concatenated_sequences.hpp>

int main()
{
    using bio::alphabet::operator""_dna4;

    bio::ranges::concatenated_sequences<bio::alphabet::dna4_vector> foobar;
    foobar.insert(foobar.end(), 2, "ACGT"_dna4);
    fmt::print("{}\n", foobar[0]); // "ACGT"
    fmt::print("{}\n", foobar[1]); // "ACGT"
}
