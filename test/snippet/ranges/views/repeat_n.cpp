#include <string>

#include <bio/alphabet/fmt.hpp>
#include <bio/ranges/views/repeat_n.hpp>
#include <ranges>

int main()
{
    auto v = bio::views::repeat_n(std::string{"foo"}, 5);

    fmt::print("{}\n", v.size()); // prints 5
    fmt::print("{}\n", v);        // prints ["foo", "foo", "foo", "foo", "foo"]

    v[0] = std::string{"foobar"};

    // Now it always prints "foobar"
    fmt::print("{}\n", *std::ranges::begin(v)); // prints "foobar"
    fmt::print("{}\n", v.size());               // prints 5; Note: the size cannot be changed anymore
}
