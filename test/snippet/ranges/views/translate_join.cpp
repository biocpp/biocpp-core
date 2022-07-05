//! [example]
#include <iostream>

#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/alphabet/fmt.hpp>
#include <bio/ranges/views/char_to.hpp>
#include <bio/ranges/views/translate_join.hpp>

using bio::alphabet::operator""_dna4;

int main()
{
    // Input range needs to be two-dimensional
    std::vector<std::vector<bio::alphabet::dna4> > vec{"ACGTACGTACGTA"_dna4, "TCGAGAGCTTTAGC"_dna4};

    // Translation with default parameters
    auto v1 = vec | bio::views::translate_join;
    fmt::print("{}\n", v1); // [TYVR,RTYV,VRT,YVRT,TYVR,RTY,SRAL,REL*,ESFS,AKAL,LKLS,*SSR]

    // Access the third forward frame (index_frame 2) of the second input sequence (index_seq 1)
    // Required frames per sequence s = 6
    // n = (index_seq * s) + j
    //   = 1 * 6 + 2
    //   = 8

    auto third_frame_second_seq = v1[1 * 6 + 2];
    fmt::print("{}\n", third_frame_second_seq); // ESFS

    // Translation with custom translation frame
    auto v2 = vec | bio::views::translate_join(bio::alphabet::translation_frames::FWD_FRAME_0);
    fmt::print("{}\n", v2); // [TYVR,SRAL]

    return 0;
}
//! [example]
