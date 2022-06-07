#include <string>

#include <bio/alphabet/fmt.hpp>
#include <bio/ranges/views/take_exactly.hpp>           // provides views::take_exactly and views::take_exactly_or_throw

int main()
{
    std::string vec{"foobar"};
    auto v = vec | bio::views::take_exactly(3);              // or bio::views::take_exactly_or_throw
    fmt::print("{}\n", v);                          // "foo"
    fmt::print("{}\n", std::ranges::size(v));       // 3


    auto v2 = vec | bio::views::take_exactly(9);
    fmt::print("{}\n", std::ranges::size(v2));      // 9 <- here be dragons!

}
