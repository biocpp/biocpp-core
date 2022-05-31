#include <bio/alphabet/nucleotide/rna4.hpp>
#include <bio/meta/debug_stream.hpp>

int main()
{
    using bio::operator""_rna4;

    bio::rna4 my_letter{'A'_rna4};

    my_letter.assign_char('C');

    my_letter.assign_char('F'); // unknown characters are implicitly converted to A.
    if (my_letter.to_char() == 'A')
        bio::debug_stream << "yeah\n"; // "yeah";
}
