#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/meta/debug_stream.hpp>
#include <bio/ranges/container/concatenated_sequences.hpp>

int main()
{
    using bio::operator""_dna4;

    bio::concatenated_sequences<bio::dna4_vector> foobar;
    foobar.insert(foobar.end(), "ACGT"_dna4);
    bio::debug_stream << foobar[0] << '\n'; // "ACGT"
}
