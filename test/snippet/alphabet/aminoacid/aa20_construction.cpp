#include <bio/alphabet/aminoacid/aa20.hpp>
#include <bio/meta/debug_stream.hpp>

int main()
{
    using bio::operator""_aa20;

    bio::aa20 my_letter{'A'_aa20};
    
    my_letter.assign_char('C');

    my_letter.assign_char('?'); // all unknown characters are converted to 'A'_aa20 implicitly

    if (my_letter.to_char() == 'A')
        bio::debug_stream << "yeah\n"; // "yeah";
}
