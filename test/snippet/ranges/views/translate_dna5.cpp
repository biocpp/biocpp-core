#include <bio/alphabet/nucleotide/dna5.hpp>
#include <bio/alphabet/fmt.hpp>
#include <bio/ranges/views/complement.hpp>
#include <bio/ranges/views/translate_single.hpp>

int main()
{
    using namespace bio::alphabet::literals;

    std::vector<bio::alphabet::dna5> vec{"ACGTACGTACGTA"_dna5};

    // Default (first forward frame)
    auto v1 = vec | bio::views::translate_single;
    // == [T,Y,V,R]
    fmt::print("{}\n", v1[1]);

    // First forward frame
    auto v2 = vec | bio::views::translate_single(bio::alphabet::translation_frames::FWD_FRAME_0);
    // == [T,Y,V,R]

    // First reverse frame
    auto v3 = vec | bio::views::translate_single(bio::alphabet::translation_frames::REV_FRAME_0);
    // == [Y,V,R,T]

    // Second forward frame
    auto v4 = vec | bio::views::translate_single(bio::alphabet::translation_frames::FWD_FRAME_1);
    // == [R,T,Y,V]

    // Second reverse frame
    auto v5 = vec | bio::views::translate_single(bio::alphabet::translation_frames::REV_FRAME_1);
    // == [T,Y,V,R]

    // Third forward frame
    auto v6 = vec | bio::views::translate_single(bio::alphabet::translation_frames::FWD_FRAME_2);
    // == [V,R,T]

    // Third reverse frame
    auto v7 = vec | bio::views::translate_single(bio::alphabet::translation_frames::REV_FRAME_2);
    // == [R,T,Y]

    // function syntax
    auto v8 = bio::views::translate_single(vec, bio::alphabet::translation_frames::FWD_FRAME_0);
    // == [T,Y,V,R]

    // combinability
    auto v9 = vec | bio::views::complement | bio::views::translate_single(bio::alphabet::translation_frames::REV_FRAME_0);
    // == [M,H,A,C]

    // combinability with default parameter
    auto v10 = vec | bio::views::complement | bio::views::translate_single;
    // == [C,M,H,A]

    // combinability with default parameter
    auto v11 = vec | bio::views::complement | bio::views::translate_single();
    // == [C,M,H,A]
}
