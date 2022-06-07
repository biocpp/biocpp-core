#include <bio/alphabet/nucleotide/rna15.hpp>
#include <bio/alphabet/fmt.hpp>

int main()
{
    using bio::operator""_rna15;

    bio::rna15 my_letter{'A'_rna15};

    my_letter.assign_char('C');

    my_letter.assign_char('F'); // unknown characters are implicitly converted to N.
    if (my_letter.to_char() == 'N')
        fmt::print("{}", "yeah\n"); // "yeah";
}
