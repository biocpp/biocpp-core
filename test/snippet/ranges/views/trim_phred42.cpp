#include <string>
#include <vector>

#include <bio/alphabet/quality/phred42.hpp>
#include <bio/ranges/views/trim_quality.hpp>
#include <bio/ranges/views/to_char.hpp>

int main()
{
    std::vector<bio::phred42> vec{bio::phred42{40}, bio::phred42{40}, bio::phred42{30},
                                     bio::phred42{20}, bio::phred42{10}};

    // trim by phred_value
    auto v1 = vec | bio::views::trim_quality(20u);                            // == ['I','I','?','5']

    // trim by quality character
    auto v2 = vec | bio::views::trim_quality(bio::phred42{40});            // == ['I','I']

    // function syntax
    auto v3 = bio::views::trim_quality(vec, 20u);                             // == ['I','I','?','5']

    // combinability
    auto v4 = bio::views::trim_quality(vec, 20u) | bio::views::to_char;    // == "II?5"
}
