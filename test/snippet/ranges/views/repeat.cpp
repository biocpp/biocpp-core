#include <bio/meta/debug_stream.hpp>
#include <bio/ranges/views/repeat.hpp>
#include <ranges>

int main()
{
    auto v = bio::views::repeat('A');

    bio::debug_stream << *std::ranges::begin(v) << '\n'; // prints 'A'
    bio::debug_stream << v[12355] << '\n';               // also prints 'A'. It always prints 'A'

    v[1345] = 'C';

    // Now it always prints 'C'
    bio::debug_stream << *std::ranges::begin(v) << '\n'; // prints 'C'
    bio::debug_stream << v[12355] << '\n';               // prints 'C'
}
