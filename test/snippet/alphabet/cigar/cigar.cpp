#include <bio/alphabet/cigar/cigar.hpp>
#include <bio/alphabet/fmt.hpp>

using bio::alphabet::operator""_cigar_op;

int main()
{
    bio::alphabet::cigar c{13, 'M'_cigar_op};
    fmt::print("{}\n", c); // "13M"

    c.assign_string("14X");
    fmt::print("{}\n", c); // "14X"
}
