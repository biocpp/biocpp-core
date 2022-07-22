#include <bio/alphabet/nucleotide/dna16sam.hpp>
#include <bio/alphabet/fmt.hpp>

int main()
{
    using namespace bio::alphabet::literals;

    bio::alphabet::dna16sam_vector foo{"ACgtTA"_dna16sam};
    bio::alphabet::dna16sam_vector bar = "ACG==A"_dna16sam;
    auto bax = "A=GTT!"_dna16sam;

    fmt::print("{}\n{}\n{}\n", foo, bar, bax);
}
