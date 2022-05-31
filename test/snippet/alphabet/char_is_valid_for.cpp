#include <bio/alphabet/concept.hpp>
#include <bio/alphabet/adaptation/char.hpp>
#include <bio/alphabet/nucleotide/dna5.hpp>

int main()
{
    bool b = bio::char_is_valid_for<char>('A');
    // calls bio::custom::char_is_valid_for<char>('A'); always true

    bool c = bio::char_is_valid_for<bio::dna5>('A');
    // calls dna5::char_is_valid('A') member; == true

    // for some alphabets, characters that are not uniquely mappable are still valid:
    bool d = bio::char_is_valid_for<bio::dna5>('a');
    // lower case also true
}
