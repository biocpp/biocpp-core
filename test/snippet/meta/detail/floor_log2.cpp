#include <bio/meta/math.hpp>
#include <bio/meta/debug_stream.hpp>

int main()
{
    // the same as std::floor(std::log2(x)), but exact for unsigned integers
    bio::debug_stream << "floor_log2(2^0 + 0): " << bio::detail::floor_log2(1u) << '\n'; // 0u
    bio::debug_stream << "floor_log2(2^1 + 0): " << bio::detail::floor_log2(2u) << '\n'; // 1u
    bio::debug_stream << "floor_log2(2^1 + 1): " << bio::detail::floor_log2(3u) << '\n'; // 1u
    bio::debug_stream << "floor_log2(2^2 + 0): " << bio::detail::floor_log2(4u) << '\n'; // 2u
    bio::debug_stream << "floor_log2(2^2 + 1): " << bio::detail::floor_log2(5u) << '\n'; // 2u
    bio::debug_stream << "floor_log2(2^2 + 2): " << bio::detail::floor_log2(6u) << '\n'; // 2u
    bio::debug_stream << "floor_log2(2^2 + 3): " << bio::detail::floor_log2(7u) << '\n'; // 2u
    bio::debug_stream << "floor_log2(2^3 + 0): " << bio::detail::floor_log2(8u) << '\n'; // 3u
    bio::debug_stream << "floor_log2(2^3 + 1): " << bio::detail::floor_log2(9u) << '\n'; // 3u

    return 0;
}
