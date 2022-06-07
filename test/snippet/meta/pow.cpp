#include <bio/meta/math.hpp>
#include <bio/alphabet/fmt.hpp>

int main()
{
    // Uses specialisation for signed integers.
    fmt::print("{}\n", bio::pow(2, 3u));                            // Prints 8
    fmt::print("{}\n", bio::pow(-2, 3u));                           // Prints -8

    // Uses specialisation for unsigned integers.
    fmt::print("{}\n", bio::pow(2u, 3u));                           // Prints 8

    // Uses `std::pow`.
    fmt::print("{}\n", bio::pow(2, 3));                             // Prints 8
    fmt::print("{}\n", bio::pow(2u, 3));                            // Prints 8
    fmt::print("{}\n", bio::pow(2.0, 3));                           // Prints 8

    // 5^25 should be 298023223876953125.
    fmt::print("{}\n", bio::pow(5u, 25u));                          // Prints 298023223876953125
    fmt::print("{}\n", static_cast<uint64_t>(std::pow(5u, 25u)));   // Prints 298023223876953152 (wrong!)
}
