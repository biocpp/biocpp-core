#include <bio/alphabet/concept.hpp>
#include <bio/alphabet/custom/char.hpp>
#include <bio/alphabet/nucleotide/dna5.hpp>

int main()
{
    char c = '!';
    bio::alphabet::assign_rank_to(66, c);     // calls bio::alphabet::custom::tag_invoke(bio::alphabet::custom::assign_rank_to, 66, c); == 'B'

    bio::alphabet::dna5 d{};
    bio::alphabet::assign_rank_to(2, d);     // calls .assign_rank(2) member; == 'G'_dna5

    // also works for temporaries:
    bio::alphabet::dna5 d2 = bio::alphabet::assign_rank_to(2, bio::alphabet::dna5{});

    // too-large ranks are undefined behaviour:
    // bio::alphabet::dna5 d3 = bio::alphabet::assign_rank_to(50, bio::alphabet::dna5{});
}
