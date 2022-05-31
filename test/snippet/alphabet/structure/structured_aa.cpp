#include <bio/alphabet/structure/structured_aa.hpp>
#include <bio/alphabet/aminoacid/aa27.hpp>
#include <bio/meta/debug_stream.hpp>

int main()
{
    using bio::operator""_aa27;
    using bio::operator""_dssp9;
    using bio::get;

    bio::structured_aa<bio::aa27, bio::dssp9> letter{'W'_aa27, 'B'_dssp9};

    bio::debug_stream << bio::to_rank(letter) << ' '
                         << bio::to_rank(get<0>(letter)) << ' '
                         << bio::to_rank(get<1>(letter)) << '\n';
    // 49 22 1

    bio::debug_stream << bio::to_char(letter) << ' '
                         << bio::to_char(get<0>(letter)) << ' '
                         << bio::to_char(get<1>(letter)) << '\n';
    // W W B

    // modify via structured bindings and references:
    auto & [ seq_l, structure_l ] = letter;
    seq_l = 'V'_aa27;
    bio::debug_stream << bio::to_char(letter) << '\n';
    // V
}
