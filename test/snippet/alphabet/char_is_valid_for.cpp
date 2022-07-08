#include <bio/alphabet/concept.hpp>
#include <bio/alphabet/adaptation/char.hpp>
#include <bio/alphabet/nucleotide/dna5.hpp>

int main()
{
    bool b = bio::alphabet::char_is_valid_for<char>('A');
    // calls bio::alphabet::cpo::tag_invoke(bio::alphabet::cpo::char_is_valid_for, 'A', char{}); always true

    bool c = bio::alphabet::char_is_valid_for<bio::alphabet::dna5>('A');
    // calls dna5::char_is_valid('A') member; == true

    // for some alphabets, characters that are not uniquely mappable are still valid:
    bool d = bio::alphabet::char_is_valid_for<bio::alphabet::dna5>('a');
    // lower case also true
}
