#include <bio/alphabet/cigar/cigar_op.hpp>
#include <bio/meta/debug_stream.hpp>

using seqan3::operator""_cigar_op;

int main()
{
    // Initialise a seqan3::cigar_op:
    seqan3::cigar_op match{'M'_cigar_op};

    // you can print cigar_op values:
    seqan3::debug_stream << match << '\n'; // M
}
