#include <bio/alphabet/aminoacid/aa10li.hpp>
#include <bio/alphabet/aminoacid/aa27.hpp>
#include <bio/alphabet/fmt.hpp>
#include <bio/ranges/views/convert.hpp>

using namespace bio::alphabet::literals;

int main()
{
    // Construction of aa10li amino acids from character
    bio::alphabet::aa10li my_letter{'A'_aa10li};

    my_letter.assign_char('C');
    my_letter.assign_char('?'); // all unknown characters are converted to 'S'_aa10li implicitly

    if (my_letter.to_char() == 'S')
        fmt::print("{}", "yeah\n"); // "yeah";

    // Convert aa27 alphabet to aa10_murphy
    std::vector<bio::alphabet::aa27> v1{"ALRSTXOUMP"_aa27};
    auto v2 = v1 | bio::views::convert<bio::alphabet::aa10li>; // AJKAASKCJP

    fmt::print("{}\n", v2);

    return 0;
}
