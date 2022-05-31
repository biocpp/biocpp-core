#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/meta/debug_stream.hpp>
#include <bio/ranges/container/concatenated_sequences.hpp>

int main()
{
    using seqan3::operator""_dna4;

    seqan3::concatenated_sequences<seqan3::dna4_vector> foobar;
    foobar.insert(foobar.end(), "ACGT"_dna4);
    seqan3::debug_stream << foobar[0] << '\n'; // "ACGT"
}
