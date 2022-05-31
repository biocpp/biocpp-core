#include <bio/alphabet/structure/structured_rna.hpp>
#include <bio/alphabet/nucleotide/rna4.hpp>
#include <bio/alphabet/structure/dot_bracket3.hpp>
#include <bio/meta/debug_stream.hpp>

int main()
{
    using bio::operator""_rna4;
    using bio::operator""_db3;
    using bio::get;

    bio::structured_rna<bio::rna4, bio::dot_bracket3> letter{'G'_rna4, '('_db3};
    bio::debug_stream << bio::to_rank(letter) << ' '
                         << bio::to_rank(get<0>(letter)) << ' '
                         << bio::to_rank(get<1>(letter)) << '\n';
    // 6 2 1

    bio::debug_stream << bio::to_char(letter) << ' '
                         << bio::to_char(get<0>(letter)) << ' '
                         << bio::to_char(get<1>(letter)) << '\n';
    // G G (

    // modify via structured bindings and references:
    auto & [ seq_l, structure_l ] = letter;
    seq_l = 'U'_rna4;
    bio::debug_stream << bio::to_char(letter) << '\n';
    // U
}
