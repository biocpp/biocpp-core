#include <bio/alphabet/aminoacid/aa27.hpp>
#include <bio/meta/debug_stream.hpp>

int main()
{
    using bio::operator""_aa27;

    bio::aa27 my_letter{'A'_aa27};

    my_letter.assign_char('C');
    
    my_letter.assign_char('?'); // all unknown characters are converted to 'X'_aa27 implicitly
    if (my_letter.to_char() == 'X')
        bio::debug_stream << "yeah\n"; // "yeah";
}
