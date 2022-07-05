#include <string>

#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/alphabet/nucleotide/dna5.hpp>
#include <bio/ranges/views/char_to.hpp>

int main()
{
    std::string s{"ACTTTGATAN"};
    auto v1 = s | bio::views::char_to<bio::alphabet::dna4>; // == "ACTTTGATAA"_dna4
    auto v2 = s | bio::views::char_to<bio::alphabet::dna5>; // == "ACTTTGATAN"_dna5
}
