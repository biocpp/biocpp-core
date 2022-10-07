#include <vector>

#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/alphabet/quality/phred42.hpp>
#include <bio/alphabet/quality/aliases.hpp>
#include <bio/alphabet/fmt.hpp>
#include <bio/ranges/views/to_rank.hpp>
#include <bio/ranges/views/convert.hpp>

int main()
{
    using namespace bio::alphabet::literals;

    std::vector<bio::alphabet::dna4> vec = "ACTTTGATA"_dna4;
    auto v = vec | bio::views::to_rank | bio::views::convert<unsigned>;
    fmt::print("{}\n", v); // [0,1,3,3,3,2,0,3,0]

    std::vector<bio::alphabet::phred42> qvec{{0}, {7}, {5}, {3}, {7}, {4}, {30}, {16}, {23}};
    auto v3 = qvec | bio::views::to_rank | bio::views::convert<unsigned>;
    fmt::print("{}\n", v3); // [0,7,5,3,7,4,30,16,23]

    std::vector<bio::alphabet::dna4q> qcvec{{'C'_dna4, bio::alphabet::phred42{0}}, {'A'_dna4, bio::alphabet::phred42{7}},
                                     {'G'_dna4, bio::alphabet::phred42{5}}, {'T'_dna4, bio::alphabet::phred42{3}},
                                     {'G'_dna4, bio::alphabet::phred42{7}}, {'A'_dna4, bio::alphabet::phred42{4}},
                                     {'C'_dna4, bio::alphabet::phred42{30}}, {'T'_dna4, bio::alphabet::phred42{16}},
                                     {'A'_dna4, bio::alphabet::phred42{23}}};
    auto v4 = qcvec | bio::views::to_rank | bio::views::convert<unsigned>;
    fmt::print("{}\n", v4); // [1,28,22,15,30,16,121,67,92]
}
