#include <bio/alphabet/nucleotide/dna16sam.hpp>
#include <bio/alphabet/fmt.hpp>

int main()
{
    using namespace bio::alphabet::literals;

    std::vector<bio::alphabet::dna16sam> foo = "ACG==A"_dna16sam;
    auto bar = "A=GTTC"_dna16sam;

    fmt::print("{}\n{}\n", foo, bar);
}
