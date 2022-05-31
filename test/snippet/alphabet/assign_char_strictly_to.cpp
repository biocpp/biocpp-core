#include <bio/alphabet/concept.hpp>
#include <bio/alphabet/adaptation/char.hpp>
#include <bio/alphabet/nucleotide/dna5.hpp>

int main()
{
    char c = '!';
    bio::assign_char_strictly_to('?', c);     // calls bio::custom::assign_char_strictly_to('A', c)

    bio::dna5 d{};
    bio::assign_char_strictly_to('A', d);     // calls .assign_char('A') member

    // also works for temporaries:
    bio::dna5 d2 = bio::assign_char_strictly_to('A', bio::dna5{});
}
