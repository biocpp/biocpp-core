#include <bio/alphabet/cigar/cigar.hpp>
#include <bio/meta/debug_stream.hpp>

using bio::operator""_cigar_op;

int main()
{
    bio::cigar c{13, 'M'_cigar_op};
    bio::debug_stream << c << '\n'; // "13M"

    c.assign_string("14X");
    bio::debug_stream << c << '\n'; // "14X"
}
