#include <bio/alphabet/cigar/cigar_op.hpp>
#include <bio/alphabet/fmt.hpp>

using namespace bio::alphabet::literals;

int main()
{
    // Initialise a bio::alphabet::cigar_op:
    bio::alphabet::cigar_op match{'M'_cigar_op};

    // you can print cigar_op values:
    fmt::print("{}\n", match); // M
}
