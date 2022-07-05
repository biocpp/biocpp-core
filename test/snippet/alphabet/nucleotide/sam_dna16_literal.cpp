#include <bio/alphabet/nucleotide/sam_dna16.hpp>
#include <bio/alphabet/fmt.hpp>

int main()
{
    using bio::alphabet::operator""_sam_dna16;

    bio::alphabet::sam_dna16_vector foo{"ACgtTA"_sam_dna16};
    bio::alphabet::sam_dna16_vector bar = "ACG==A"_sam_dna16;
    auto bax = "A=GTT!"_sam_dna16;

    fmt::print("{}\n{}\n{}\n", foo, bar, bax);
}
