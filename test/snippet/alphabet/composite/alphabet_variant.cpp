#include <bio/alphabet/composite/alphabet_variant.hpp>
#include <bio/alphabet/gap/gap.hpp>
#include <bio/alphabet/nucleotide/all.hpp>

int main()
{
    using bio::operator""_dna5;
    using bio::operator""_rna5;

    bio::alphabet_variant<bio::dna5, bio::gap> letter{};          // implicitly 'A'_dna5
    bio::alphabet_variant<bio::dna5, bio::gap> letter2{'C'_dna5}; // constructed from alternative (== 'C'_dna5)
    bio::alphabet_variant<bio::dna5, bio::gap> letter3{'U'_rna5}; // constructed from type that alternative is constructible from (== 'T'_dna5)

    letter2.assign_char('T');                       // == 'T'_dna5
    letter2.assign_char('-');                       // == gap{}
    letter2.assign_char('K');                       // unknown characters map to the default/unknown
                                                    // character of the first alternative type (== 'N'_dna5)

    letter2 = bio::gap{};                        // assigned from alternative (== gap{})
    letter2 = 'U'_rna5;                             // assigned from type that alternative is assignable from (== 'T'_dna5)

    bio::dna5 letter4 = letter2.convert_to<bio::dna5>();
}
