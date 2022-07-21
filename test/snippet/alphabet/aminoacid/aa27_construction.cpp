#include <bio/alphabet/aminoacid/aa27.hpp>
#include <bio/alphabet/fmt.hpp>

int main()
{
    using namespace bio::alphabet::literals;

    bio::alphabet::aa27 my_letter{'A'_aa27};

    my_letter.assign_char('C');

    my_letter.assign_char('?'); // all unknown characters are converted to 'X'_aa27 implicitly
    if (my_letter.to_char() == 'X')
        fmt::print("{}", "yeah\n"); // "yeah";
}
