#include <bio/meta/math.hpp>
#include <bio/alphabet/fmt.hpp>

int main()
{
    // the same as std::floor(std::log2(x)), but exact for unsigned integers
    fmt::print("floor_log2(2^0 + 0): ", bio::detail::floor_log2(1u)); // 0u
    fmt::print("floor_log2(2^1 + 0): ", bio::detail::floor_log2(2u)); // 1u
    fmt::print("floor_log2(2^1 + 1): ", bio::detail::floor_log2(3u)); // 1u
    fmt::print("floor_log2(2^2 + 0): ", bio::detail::floor_log2(4u)); // 2u
    fmt::print("floor_log2(2^2 + 1): ", bio::detail::floor_log2(5u)); // 2u
    fmt::print("floor_log2(2^2 + 2): ", bio::detail::floor_log2(6u)); // 2u
    fmt::print("floor_log2(2^2 + 3): ", bio::detail::floor_log2(7u)); // 2u
    fmt::print("floor_log2(2^3 + 0): ", bio::detail::floor_log2(8u)); // 3u
    fmt::print("floor_log2(2^3 + 1): ", bio::detail::floor_log2(9u)); // 3u

    return 0;
}
