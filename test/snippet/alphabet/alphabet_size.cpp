#include <bio/alphabet/concept.hpp>
#include <bio/alphabet/custom/char.hpp>
#include <bio/alphabet/nucleotide/dna5.hpp>

int main()
{
    // calls bio::alphabet::custom::tag_invoke(bio::alphabet::custom::size, char{}); r2 == 256
    auto r2 = bio::alphabet::size<char>;

    // calls bio::alphabet::base's friend tag_invoke() which returns dna5::alphabet_size == 5
    auto r3 = bio::alphabet::size<bio::alphabet::dna5>;
}
