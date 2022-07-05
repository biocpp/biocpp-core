#include <bio/alphabet/nucleotide/dna15.hpp>
#include <bio/alphabet/nucleotide/dna5.hpp>
#include <bio/ranges/views/convert.hpp>

int main()
{
    using bio::alphabet::operator""_dna15;

    bio::alphabet::dna15_vector vec2{"ACYGTN"_dna15};
    auto v4 = vec2 | bio::views::convert<bio::alphabet::dna5>; // == "ACNGTN"_dna5
}
