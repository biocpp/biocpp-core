#include <bio/alphabet/nucleotide/rna5.hpp>
#include <bio/alphabet/fmt.hpp>
int main()
{
    using bio::alphabet::operator""_rna5;

    bio::alphabet::rna5 my_letter{'A'_rna5};

    my_letter.assign_char('C');

    my_letter.assign_char('F'); // unknown characters are implicitly converted to N.
    if (my_letter.to_char() == 'N')
        fmt::print("{}", "yeah\n"); // "yeah";
}
