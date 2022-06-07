#include <bio/alphabet/aminoacid/aa10murphy.hpp>
#include <bio/alphabet/aminoacid/aa27.hpp>
#include <bio/alphabet/fmt.hpp>
#include <bio/ranges/views/convert.hpp>

using bio::operator""_aa10murphy;
using bio::operator""_aa27;

int main()
{
    // Construction of aa10murphy amino acids from character
    bio::aa10murphy my_letter{'A'_aa10murphy};

    my_letter.assign_char('C');
    my_letter.assign_char('?'); // all unknown characters are converted to 'S'_aa10murphy implicitly

    if (my_letter.to_char() == 'S')
        fmt::print("{}", "yeah\n"); // "yeah";

    // Convert aa27 alphabet to aa10_murphy
    bio::aa27_vector v1{"AVRSTXOUB"_aa27};
    auto v2 = v1 | bio::views::convert<bio::aa10murphy>; // AIKSSSKCB

    fmt::print("{}\n", v2);

    return 0;
}
