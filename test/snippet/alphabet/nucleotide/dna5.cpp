#include <bio/alphabet/nucleotide/dna5.hpp>
#include <bio/alphabet/fmt.hpp>

int main()
{
    using bio::operator""_dna5;

    bio::dna5 my_letter{'A'_dna5};

    my_letter.assign_char('C');

    my_letter.assign_char('F'); // unknown characters are implicitly converted to N.
    if (my_letter.to_char() == 'N')
        fmt::print("{}", "yeah\n"); // "yeah";
}
