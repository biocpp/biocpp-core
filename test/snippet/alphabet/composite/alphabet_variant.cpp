#include <bio/alphabet/composite/alphabet_variant.hpp>
#include <bio/alphabet/gap/gap.hpp>
#include <bio/alphabet/nucleotide/all.hpp>

int main()
{
    using namespace bio::alphabet::literals;
    using namespace bio::alphabet::literals;

    bio::alphabet::alphabet_variant<bio::alphabet::dna5, bio::alphabet::gap> letter{};          // implicitly 'A'_dna5
    bio::alphabet::alphabet_variant<bio::alphabet::dna5, bio::alphabet::gap> letter2{'C'_dna5}; // constructed from alternative (== 'C'_dna5)
    bio::alphabet::alphabet_variant<bio::alphabet::dna5, bio::alphabet::gap> letter3{'U'_rna5}; // constructed from type that alternative is constructible from (== 'T'_dna5)

    letter2.assign_char('T');                       // == 'T'_dna5
    letter2.assign_char('-');                       // == gap{}
    letter2.assign_char('K');                       // unknown characters map to the default/unknown
                                                    // character of the first alternative type (== 'N'_dna5)

    letter2 = bio::alphabet::gap{};                        // assigned from alternative (== gap{})
    letter2 = 'U'_rna5;                             // assigned from type that alternative is assignable from (== 'T'_dna5)

    bio::alphabet::dna5 letter4 = letter2.convert_to<bio::alphabet::dna5>();
}
