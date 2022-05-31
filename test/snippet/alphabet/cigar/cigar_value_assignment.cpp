#include <bio/alphabet/cigar/cigar.hpp>
#include <bio/alphabet/cigar/cigar_op.hpp>
#include <bio/meta/debug_stream.hpp>

int main()
{
    using bio::operator""_cigar_op;

    bio::cigar letter{10, 'I'_cigar_op};

    letter = 'D'_cigar_op;  // yields 10D
    letter = 20; // yields 20D

    if (letter == bio::cigar{20, 'D'_cigar_op})
        bio::debug_stream << "yeah\n";
}
