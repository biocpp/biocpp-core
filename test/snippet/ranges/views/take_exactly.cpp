#include <string>

#include <bio/meta/debug_stream.hpp>
#include <bio/ranges/views/take_exactly.hpp>           // provides views::take_exactly and views::take_exactly_or_throw

int main()
{
    std::string vec{"foobar"};
    auto v = vec | bio::views::take_exactly(3);              // or bio::views::take_exactly_or_throw
    bio::debug_stream << v << '\n';                          // "foo"
    bio::debug_stream << std::ranges::size(v) << '\n';       // 3


    auto v2 = vec | bio::views::take_exactly(9);
    bio::debug_stream << std::ranges::size(v2) << '\n';      // 9 <- here be dragons!

}
