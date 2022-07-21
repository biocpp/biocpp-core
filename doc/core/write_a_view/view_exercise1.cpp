//![start]
#include <iostream>
#include <ranges>
#include <bio/alphabet/nucleotide/all.hpp>

using namespace bio::alphabet::literals;

//![start]
auto const my_convert_to_char_view = std::views::transform([] (auto const alph)
{
    return bio::alphabet::to_char(alph);
});

//![end]
int main()
{
    std::vector<bio::alphabet::dna5> vec{"ATTAGATTA"_dna5};
    // std::cout << vec[0] << '\n';                 // won't work

    auto v = vec | my_convert_to_char_view;

    std::cout << v[0] << '\n';                      // prints "A"
}
//![end]
