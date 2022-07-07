#include <bio/alphabet/concept.hpp>
#include <bio/alphabet/adaptation/char.hpp>
#include <bio/alphabet/nucleotide/dna5.hpp>

int main()
{
    // calls bio::alphabet::cpo::tag_invoke(bio::alphabet::cpo::size, char{}); r2 == 256
    auto r2 = bio::alphabet::alphabet_size<char>;

    // calls bio::alphabet::alphabet_base's friend tag_invoke() which returns dna5::alphabet_size == 5
    auto r3 = bio::alphabet::alphabet_size<bio::alphabet::dna5>;
}
