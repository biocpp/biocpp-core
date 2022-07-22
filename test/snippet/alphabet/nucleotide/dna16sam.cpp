#include <bio/alphabet/nucleotide/dna16sam.hpp>
#include <bio/alphabet/fmt.hpp>

int main()
{
    using namespace bio::alphabet::literals;

    bio::alphabet::dna16sam my_letter{'A'_dna16sam};

    my_letter.assign_char('=');

    my_letter.assign_char('F'); // unknown characters are implicitly converted to N.
    fmt::print("{}\n", my_letter); // "N";
}
