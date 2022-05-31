#include <string>

#include <bio/meta/debug_stream.hpp>
#include <bio/ranges/views/repeat_n.hpp>
#include <ranges>

int main()
{
    auto v = seqan3::views::repeat_n(std::string{"foo"}, 5);

    seqan3::debug_stream << v.size() << '\n'; // prints 5
    seqan3::debug_stream << v << '\n';        // prints ["foo", "foo", "foo", "foo", "foo"]

    v[0] = std::string{"foobar"};

    // Now it always prints "foobar"
    seqan3::debug_stream << *std::ranges::begin(v) << '\n'; // prints "foobar"
    seqan3::debug_stream << v.size() << '\n';               // prints 5; Note: the size cannot be changed anymore
}
