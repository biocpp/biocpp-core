#include <bio/alphabet/quality/phred68legacy.hpp>
#include <bio/alphabet/fmt.hpp>
#include <algorithm>

int main()
{
    using namespace bio::alphabet::literals;

    // directly assign to a std::vector<phred68legacy> using a string literal
    std::vector<bio::alphabet::phred68legacy> qual_vec = "###!"_phred68legacy;

    // This is the same as a sequence of char literals:
    std::vector<bio::alphabet::phred68legacy> qual_vec2 = {'#'_phred68legacy, '#'_phred68legacy,
                                                    '#'_phred68legacy, '!'_phred68legacy};

    fmt::print("{}\n", std::ranges::equal(qual_vec, qual_vec2)); // prints 1 (true)
}
