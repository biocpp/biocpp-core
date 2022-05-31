#include <string>

#include <bio/meta/debug_stream.hpp>
#include <bio/ranges/views/slice.hpp>     // provides views::slice
#include <ranges>                // provides std::views::reverse

int main()
{
    std::string s{"foobar"};
    auto v = s | bio::views::slice(1,4);
    bio::debug_stream << v << '\n';      // "oob"

    auto v2 = s | std::views::reverse | bio::views::slice(1, 4);
    bio::debug_stream << v2 << '\n';     // "abo"

}
