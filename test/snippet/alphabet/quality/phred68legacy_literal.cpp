#include <bio/alphabet/quality/phred68legacy.hpp>
#include <bio/meta/debug_stream.hpp>
#include <algorithm>

int main()
{
    using bio::operator""_phred68legacy;

    // directly assign to a std::vector<phred68legacy> using a string literal
    std::vector<bio::phred68legacy> qual_vec = "###!"_phred68legacy;

    // This is the same as a sequence of char literals:
    std::vector<bio::phred68legacy> qual_vec2 = {'#'_phred68legacy, '#'_phred68legacy,
                                                    '#'_phred68legacy, '!'_phred68legacy};

    bio::debug_stream << std::ranges::equal(qual_vec, qual_vec2) << '\n'; // prints 1 (true)
}
