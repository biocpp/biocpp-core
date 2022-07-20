#include <bio/alphabet/cigar/cigar.hpp>
#include <bio/alphabet/cigar/cigar_op.hpp>
#include <bio/alphabet/fmt.hpp>

int main()
{
    using namespace bio::alphabet::literals;

    bio::alphabet::cigar letter{10, 'I'_cigar_op};

    letter = 'D'_cigar_op;  // yields 10D
    letter = 20; // yields 20D

    if (letter == bio::alphabet::cigar{20, 'D'_cigar_op})
        fmt::print("{}", "yeah\n");
}
