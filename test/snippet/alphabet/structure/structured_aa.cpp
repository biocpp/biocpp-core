#include <bio/alphabet/structure/structured_aa.hpp>
#include <bio/alphabet/aminoacid/aa27.hpp>
#include <bio/alphabet/fmt.hpp>

int main()
{
    using bio::operator""_aa27;
    using bio::operator""_dssp9;
    using bio::get;

    bio::structured_aa<bio::aa27, bio::dssp9> letter{'W'_aa27, 'B'_dssp9};

    fmt::print("{} {} {}\n",
               bio::to_rank(letter) ,
               bio::to_rank(get<0>(letter)),
               bio::to_rank(get<1>(letter)));
    // 49 22 1

    fmt::print("{} {} {}\n",
               bio::to_char(letter),
               bio::to_char(get<0>(letter)),
               bio::to_char(get<1>(letter)));
    // W W B

    // modify via structured bindings and references:
    auto & [ seq_l, structure_l ] = letter;
    seq_l = 'V'_aa27;
    fmt::print("{}\n", bio::to_char(letter));
    // V
}
