#include <bio/alphabet/cigar/cigar_op.hpp>
#include <bio/meta/debug_stream.hpp>

using bio::operator""_cigar_op;

int main()
{
    // Initialise a bio::cigar_op:
    bio::cigar_op match{'M'_cigar_op};

    // you can print cigar_op values:
    bio::debug_stream << match << '\n'; // M
}
