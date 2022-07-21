#include <bio/alphabet/nucleotide/rna4.hpp>
#include <bio/alphabet/fmt.hpp>

int main()
{
    using namespace bio::alphabet::literals;

    bio::alphabet::rna4 my_letter{'A'_rna4};

    my_letter.assign_char('C');

    my_letter.assign_char('F'); // unknown characters are implicitly converted to A.
    if (my_letter.to_char() == 'A')
        fmt::print("{}", "yeah\n"); // "yeah";
}
