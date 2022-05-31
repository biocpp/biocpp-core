#include <bio/alphabet/concept.hpp>
#include <bio/alphabet/adaptation/char.hpp>
#include <bio/alphabet/nucleotide/dna5.hpp>

int main()
{
    char c = '!';
    bio::assign_rank_to(66, c);     // calls bio::custom::assign_rank_to(66, c); == 'B'

    bio::dna5 d{};
    bio::assign_rank_to(2, d);     // calls .assign_rank(2) member; == 'G'_dna5

    // also works for temporaries:
    bio::dna5 d2 = bio::assign_rank_to(2, bio::dna5{});

    // too-large ranks are undefined behaviour:
    // bio::dna5 d3 = bio::assign_rank_to(50, bio::dna5{});
}
