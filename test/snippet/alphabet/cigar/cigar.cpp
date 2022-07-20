#include <bio/alphabet/cigar/cigar.hpp>
#include <bio/alphabet/fmt.hpp>

using namespace bio::alphabet::literals;

int main()
{
    bio::alphabet::cigar c{13, 'M'_cigar_op};
    fmt::print("{}\n", c); // "13M"

    c.assign_string("14X");
    fmt::print("{}\n", c); // "14X"
}
