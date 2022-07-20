#include <bio/alphabet/quality/phred42.hpp>
#include <bio/alphabet/fmt.hpp>
#include <algorithm>

int main()
{
    using namespace bio::alphabet::literals;

    // directly assign to a std::vector<phred42> using a string literal
    std::vector<bio::alphabet::phred42> qual_vec = "###!"_phred42;

    // This is the same as a sequence of char literals:
    std::vector<bio::alphabet::phred42> qual_vec2 = {'#'_phred42, '#'_phred42, '#'_phred42, '!'_phred42};

    fmt::print("{}\n", std::ranges::equal(qual_vec, qual_vec2)); // prints 1 (true)
}
