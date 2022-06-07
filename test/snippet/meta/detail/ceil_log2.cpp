#include <bio/meta/math.hpp>
#include <bio/alphabet/fmt.hpp>

int main()
{
    // the same as std::ceil(std::log2(x)), but exact for unsigned integers
    fmt::print("ceil_log2(2^0 + 0): {}\n", bio::detail::ceil_log2(1u)); // 0u
    fmt::print("ceil_log2(2^1 + 0): {}\n", bio::detail::ceil_log2(2u)); // 1u
    fmt::print("ceil_log2(2^1 + 1): {}\n", bio::detail::ceil_log2(3u)); // 2u
    fmt::print("ceil_log2(2^2 + 0): {}\n", bio::detail::ceil_log2(4u)); // 2u
    fmt::print("ceil_log2(2^2 + 1): {}\n", bio::detail::ceil_log2(5u)); // 3u
    fmt::print("ceil_log2(2^2 + 2): {}\n", bio::detail::ceil_log2(6u)); // 3u
    fmt::print("ceil_log2(2^2 + 3): {}\n", bio::detail::ceil_log2(7u)); // 3u
    fmt::print("ceil_log2(2^3 + 0): {}\n", bio::detail::ceil_log2(8u)); // 3u
    fmt::print("ceil_log2(2^3 + 1): {}\n", bio::detail::ceil_log2(9u)); // 4u

    return 0;
}
