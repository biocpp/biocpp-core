#include <bio/alphabet/quality/qualified.hpp>
#include <bio/alphabet/quality/phred42.hpp>
#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/meta/debug_stream.hpp>

int main()
{
    using bio::operator""_dna4;
    using bio::get;

    bio::qualified<bio::dna4, bio::phred42> letter{'A'_dna4, bio::phred42{7}};
    bio::debug_stream << int(bio::to_rank(letter)) << ' '
                         << int(bio::to_rank(get<0>(letter))) << ' '
                         << int(bio::to_rank(get<1>(letter))) << '\n';
    // 28 0 7

    bio::debug_stream << bio::to_char(letter) << ' '
                         << bio::to_char(get<0>(letter)) << ' '
                         << bio::to_char(get<1>(letter)) << '\n';
    // A A (

    bio::debug_stream << bio::to_phred(letter) << ' '
                         << bio::to_phred(get<1>(letter)) << '\n';
    // 7 7

    // modify via structured bindings and references:
    auto & [ seq_l, qual_l ] = letter;
    seq_l = 'G'_dna4;
    bio::debug_stream << bio::to_char(letter) << '\n';
    // G
}
