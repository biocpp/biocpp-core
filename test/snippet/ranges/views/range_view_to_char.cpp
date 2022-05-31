#include <vector>

#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/alphabet/quality/aliases.hpp> // includes bio::dna4q
#include <bio/alphabet/quality/phred42.hpp>
#include <bio/meta/debug_stream.hpp>
#include <bio/ranges/views/to_char.hpp>

int main()
{
    using bio::operator""_dna4;

    bio::dna4_vector vec = "ACTTTGATA"_dna4;
    auto v = vec | bio::views::to_char;
    bio::debug_stream << v << '\n'; // [A,C,T,T,T,G,A,T,A]

    std::vector<bio::phred42> qvec{{0}, {7}, {5}, {3}, {7}, {4}, {30}, {16}, {23}};
    auto v3 = qvec | bio::views::to_char;
    bio::debug_stream << v3 << '\n'; // [!,(,&,$,(,%,?,1,8]

    std::vector<bio::dna4q> qcvec{{'C'_dna4, bio::phred42{0}}, {'A'_dna4, bio::phred42{7}},
                                     {'G'_dna4, bio::phred42{5}}, {'T'_dna4, bio::phred42{3}},
                                     {'G'_dna4, bio::phred42{7}}, {'A'_dna4, bio::phred42{4}},
                                     {'C'_dna4, bio::phred42{30}}, {'T'_dna4, bio::phred42{16}},
                                     {'A'_dna4, bio::phred42{23}}};
    auto v4 = qcvec | bio::views::to_char;
    bio::debug_stream << v4 << '\n'; // [C,A,G,T,G,A,C,T,A]
}
