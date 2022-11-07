#include <ranges>                               // include all of the standard library's views

#include <bio/alphabet/fmt.hpp>
#include <bio/ranges/views/all.hpp>            // include all of BioCpp's views


int main(int argc, char** argv)
{
    if (argc != 2)
    {
        // add error handling here
        return 0;
    }
    std::string s = argv[1];

    auto s_as_dna = s | bio::views::char_to<bio::alphabet::dna5>;
    // Bonus:
    //auto s_as_dna = s | std::views::transform([] (char const c)
    //{
    //    return bio::alphabet::assign_char_strictly_to(c, bio::alphabet::dna5{});
    //});

    fmt::print("Original: {}\n", s_as_dna);
    fmt::print("RevComp:  {}\n", s_as_dna | std::views::reverse | bio::views::complement);
    fmt::print("Frames:   {}\n", s_as_dna | bio::views::translate);
}
