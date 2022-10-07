#include <bio/alphabet/nucleotide/dna15.hpp>
#include <bio/alphabet/nucleotide/dna5.hpp>
#include <bio/ranges/views/convert.hpp>

int main()
{
    using namespace bio::alphabet::literals;

    std::vector<bio::alphabet::dna15> vec2{"ACYGTN"_dna15};
    auto v4 = vec2 | bio::views::convert<bio::alphabet::dna5>; // == "ACNGTN"_dna5
}
