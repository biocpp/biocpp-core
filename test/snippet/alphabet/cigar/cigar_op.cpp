#include <bio/alphabet/cigar/cigar_op.hpp>
#include <bio/alphabet/fmt.hpp>

using bio::operator""_cigar_op;

int main()
{
    // Initialise a bio::cigar_op:
    bio::cigar_op match{'M'_cigar_op};

    // you can print cigar_op values:
    fmt::print("{}\n", match); // M
}
