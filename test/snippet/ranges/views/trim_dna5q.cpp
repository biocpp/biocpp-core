#include <string>
#include <vector>

#include <bio/alphabet/nucleotide/dna5.hpp>
#include <bio/alphabet/quality/aliases.hpp>
#include <bio/alphabet/quality/phred42.hpp>
#include <bio/ranges/views/trim_quality.hpp>
#include <bio/ranges/views/to_char.hpp>
#include <bio/ranges/views/to.hpp>

int main()
{
    using bio::operator""_dna5;
    using bio::operator""_phred42;
    std::vector<bio::dna5q> vec{{'A'_dna5, 'I'_phred42},
                                   {'G'_dna5, 'I'_phred42},
                                   {'G'_dna5, '?'_phred42},
                                   {'A'_dna5, '5'_phred42},
                                   {'T'_dna5, '+'_phred42}};
    std::vector<bio::dna5q> cmp{{'A'_dna5, 'I'_phred42},
                                   {'G'_dna5, 'I'_phred42},
                                   {'G'_dna5, '?'_phred42},
                                   {'A'_dna5, '5'_phred42}};

    // trim by phred_value
    auto v1 = vec | bio::views::trim_quality(20u);
    assert((v1 | bio::views::to<std::vector>()) == cmp);

    // trim by quality character; in this case the nucleotide part of the character is irrelevant
    auto v2 = vec | bio::views::trim_quality(bio::dna5q{'C'_dna5, '5'_phred42});
    assert((v2 | bio::views::to<std::vector>()) == cmp);

    // combinability
    auto v3 = bio::views::trim_quality(vec, 20u) | bio::views::to_char;
    assert(std::ranges::equal(std::string{"AGGA"}, v3 | bio::views::to<std::string>()));
}
