#include <string>

#include <bio/alphabet/fmt.hpp>
#include <bio/ranges/views/slice.hpp>     // provides views::slice
#include <ranges>                // provides std::views::reverse

int main()
{
    std::string s{"foobar"};
    auto v = s | bio::views::slice(1,4);
    fmt::print("{}\n", v);      // "oob"

    auto v2 = s | std::views::reverse | bio::views::slice(1, 4);
    fmt::print("{}\n", v2);     // "abo"

}
