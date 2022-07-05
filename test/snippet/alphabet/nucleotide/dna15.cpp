#include <bio/alphabet/nucleotide/dna15.hpp>
#include <bio/alphabet/fmt.hpp>

int main()
{
    using bio::alphabet::operator""_dna15;

    bio::alphabet::dna15 my_letter{'A'_dna15};

    my_letter.assign_char('C');

    my_letter.assign_char('F'); // unknown characters are implicitly converted to N.
    if (my_letter.to_char() == 'N')
        fmt::print("{}", "yeah\n"); // "yeah";
}
