#include <bio/alphabet/aminoacid/aa20.hpp>
#include <bio/alphabet/fmt.hpp>

int main()
{
    using namespace bio::alphabet::literals;

    bio::alphabet::aa20 my_letter{'A'_aa20};

    my_letter.assign_char('C');

    my_letter.assign_char('?'); // all unknown characters are converted to 'A'_aa20 implicitly

    if (my_letter.to_char() == 'A')
        fmt::print("{}", "yeah\n"); // "yeah";
}
