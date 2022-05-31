#include <string>

#include <bio/meta/debug_stream.hpp>
#include <bio/ranges/views/slice.hpp>     // provides views::slice
#include <ranges>                // provides std::views::reverse

int main()
{
    std::string s{"foobar"};
    auto v = s | seqan3::views::slice(1,4);
    seqan3::debug_stream << v << '\n';      // "oob"

    auto v2 = s | std::views::reverse | seqan3::views::slice(1, 4);
    seqan3::debug_stream << v2 << '\n';     // "abo"

}
