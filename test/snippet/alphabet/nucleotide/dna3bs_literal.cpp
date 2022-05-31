#include <bio/alphabet/nucleotide/dna3bs.hpp>
#include <bio/meta/debug_stream.hpp>

int main()
{
    using bio::operator""_dna3bs;

    bio::dna3bs_vector foo{"ACGTTA"_dna3bs}; // ATGTTA
    bio::dna3bs_vector bar = "ATGTTA"_dna3bs;

    if (foo == bar)
        bio::debug_stream << "yeah\n"; // "yeah";

    auto bax = "ACGTTA"_dna3bs;

    return 0;
}
