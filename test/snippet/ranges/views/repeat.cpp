#include <bio/alphabet/fmt.hpp>
#include <bio/ranges/views/repeat.hpp>
#include <ranges>

int main()
{
    auto v = bio::views::repeat('A');

    fmt::print("{}\n", *std::ranges::begin(v)); // prints 'A'
    fmt::print("{}\n", v[12355]);               // also prints 'A'. It always prints 'A'

    v[1345] = 'C';

    // Now it always prints 'C'
    fmt::print("{}\n", *std::ranges::begin(v)); // prints 'C'
    fmt::print("{}\n", v[12355]);               // prints 'C'
}
