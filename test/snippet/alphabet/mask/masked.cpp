#include <bio/alphabet/mask/masked.hpp>
#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/meta/debug_stream.hpp>

int main()
{
    using bio::operator""_dna4;

    bio::masked<bio::dna4> dna4_masked{};
    bio::masked<bio::dna4> dna4_another_masked{'A'_dna4, bio::mask::UNMASKED};
    // create a dna4 masked alphabet with an unmasked A

    dna4_masked.assign_char('a'); // assigns a masked 'A'_dna4

    if (dna4_masked.to_char() != dna4_another_masked.to_char())
    {
        bio::debug_stream << dna4_masked.to_char() << " is not the same as " <<
                                dna4_another_masked.to_char() << "\n";
    }
}
