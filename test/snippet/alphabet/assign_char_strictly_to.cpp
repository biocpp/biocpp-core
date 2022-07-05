#include <bio/alphabet/concept.hpp>
#include <bio/alphabet/adaptation/char.hpp>
#include <bio/alphabet/nucleotide/dna5.hpp>

int main()
{
    char c = '!';
    bio::alphabet::assign_char_strictly_to('?', c);     // calls bio::alphabet::custom::assign_char_strictly_to('A', c)

    bio::alphabet::dna5 d{};
    bio::alphabet::assign_char_strictly_to('A', d);     // calls .assign_char('A') member

    // also works for temporaries:
    bio::alphabet::dna5 d2 = bio::alphabet::assign_char_strictly_to('A', bio::alphabet::dna5{});
}
