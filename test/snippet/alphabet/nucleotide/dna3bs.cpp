#include <bio/alphabet/nucleotide/dna3bs.hpp>
#include <bio/meta/debug_stream.hpp>

int main()
{
    using bio::operator""_dna3bs;

    bio::dna3bs my_letter{'A'_dna3bs};

    my_letter.assign_char('C'); // all C will be converted to T.
    if (my_letter.to_char() == 'T')
        bio::debug_stream << "yeah\n"; // "yeah";

    my_letter.assign_char('F'); // unknown characters are implicitly converted to A.
    if (my_letter.to_char() == 'A')
        bio::debug_stream << "yeah\n"; // "yeah";

    return 0;
}
