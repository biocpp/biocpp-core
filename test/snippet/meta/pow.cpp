#include <bio/meta/math.hpp>
#include <bio/meta/debug_stream.hpp>

int main()
{
    // Uses specialisation for signed integers.
    bio::debug_stream << bio::pow(2, 3u) << '\n'; // Prints 8
    bio::debug_stream << bio::pow(-2, 3u) << '\n'; // Prints -8

    // Uses specialisation for unsigned integers.
    bio::debug_stream << bio::pow(2u, 3u) << '\n'; // Prints 8

    // Uses `std::pow`.
    bio::debug_stream << bio::pow(2, 3) << '\n'; // Prints 8
    bio::debug_stream << bio::pow(2u, 3) << '\n'; // Prints 8
    bio::debug_stream << bio::pow(2.0, 3) << '\n'; // Prints 8

    // 5^25 should be 298023223876953125.
    bio::debug_stream << bio::pow(5u, 25u) << '\n'; // Prints 298023223876953125
    bio::debug_stream << static_cast<uint64_t>(std::pow(5u, 25u)) << '\n'; // Prints 298023223876953152 (wrong!)
}
