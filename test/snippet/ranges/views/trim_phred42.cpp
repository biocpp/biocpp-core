#include <string>
#include <vector>

#include <bio/alphabet/quality/phred42.hpp>
#include <bio/ranges/views/trim_quality.hpp>
#include <bio/ranges/views/to_char.hpp>

int main()
{
    std::vector<seqan3::phred42> vec{seqan3::phred42{40}, seqan3::phred42{40}, seqan3::phred42{30},
                                     seqan3::phred42{20}, seqan3::phred42{10}};

    // trim by phred_value
    auto v1 = vec | seqan3::views::trim_quality(20u);                            // == ['I','I','?','5']

    // trim by quality character
    auto v2 = vec | seqan3::views::trim_quality(seqan3::phred42{40});            // == ['I','I']

    // function syntax
    auto v3 = seqan3::views::trim_quality(vec, 20u);                             // == ['I','I','?','5']

    // combinability
    auto v4 = seqan3::views::trim_quality(vec, 20u) | seqan3::views::to_char;    // == "II?5"
}
