#include <bio/alphabet/quality/phred63.hpp>
#include <bio/alphabet/fmt.hpp>
#include <algorithm>

int main()
{
    using bio::operator""_phred63;

    // directly assign to a std::vector<phred63> using a string literal
    std::vector<bio::phred63> qual_vec = "###!"_phred63;

    // This is the same as a sequence of char literals:
    std::vector<bio::phred63> qual_vec2 = {'#'_phred63, '#'_phred63, '#'_phred63, '!'_phred63};

    fmt::print("{}\n", std::ranges::equal(qual_vec, qual_vec2)); // prints 1 (true)
}
