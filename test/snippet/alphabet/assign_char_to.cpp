#include <bio/alphabet/concept.hpp>
#include <bio/alphabet/custom/char.hpp>
#include <bio/alphabet/nucleotide/dna5.hpp>

int main()
{
    char c = '!';
    bio::alphabet::assign_char_to('?', c);     // calls bio::alphabet::custom::tag_invoke(bio::alphabet::custom::assign_char_to, 'A', c)

    bio::alphabet::dna5 d{};
    bio::alphabet::assign_char_to('A', d);     // calls .assign_char('A') member

    // also works for temporaries:
    bio::alphabet::dna5 d2 = bio::alphabet::assign_char_to('A', bio::alphabet::dna5{});

    // invalid/unknown characters are converted:
    bio::alphabet::dna5 d3 = bio::alphabet::assign_char_to('!', bio::alphabet::dna5{}); // == 'N'_dna5
}
