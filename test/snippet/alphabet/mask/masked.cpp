#include <bio/alphabet/mask/masked.hpp>
#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/alphabet/fmt.hpp>

int main()
{
    using namespace bio::alphabet::literals;

    bio::alphabet::masked<bio::alphabet::dna4> dna4_masked{};
    bio::alphabet::masked<bio::alphabet::dna4> dna4_another_masked{'A'_dna4, bio::alphabet::mask::UNMASKED};
    // create a dna4 masked alphabet with an unmasked A

    dna4_masked.assign_char('a'); // assigns a masked 'A'_dna4

    if (dna4_masked.to_char() != dna4_another_masked.to_char())
        fmt::print("{} is not the same as {}\n", dna4_masked.to_char(), dna4_another_masked.to_char());
}
