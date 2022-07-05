#include <bio/alphabet/nucleotide/dna3bs.hpp>
#include <bio/alphabet/fmt.hpp>

int main()
{
    using bio::alphabet::operator""_dna3bs;

    bio::alphabet::dna3bs my_letter{'A'_dna3bs};

    my_letter.assign_char('C'); // all C will be converted to T.
    if (my_letter.to_char() == 'T')
        fmt::print("{}", "yeah\n"); // "yeah";

    my_letter.assign_char('F'); // unknown characters are implicitly converted to A.
    if (my_letter.to_char() == 'A')
        fmt::print("{}", "yeah\n"); // "yeah";

    return 0;
}
