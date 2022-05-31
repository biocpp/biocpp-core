#include <bio/meta/math.hpp>
#include <bio/meta/debug_stream.hpp>

int main()
{
    // the same as std::ceil(std::log2(x)), but exact for unsigned integers
    seqan3::debug_stream << "ceil_log2(2^0 + 0): " << seqan3::detail::ceil_log2(1u) << '\n'; // 0u
    seqan3::debug_stream << "ceil_log2(2^1 + 0): " << seqan3::detail::ceil_log2(2u) << '\n'; // 1u
    seqan3::debug_stream << "ceil_log2(2^1 + 1): " << seqan3::detail::ceil_log2(3u) << '\n'; // 2u
    seqan3::debug_stream << "ceil_log2(2^2 + 0): " << seqan3::detail::ceil_log2(4u) << '\n'; // 2u
    seqan3::debug_stream << "ceil_log2(2^2 + 1): " << seqan3::detail::ceil_log2(5u) << '\n'; // 3u
    seqan3::debug_stream << "ceil_log2(2^2 + 2): " << seqan3::detail::ceil_log2(6u) << '\n'; // 3u
    seqan3::debug_stream << "ceil_log2(2^2 + 3): " << seqan3::detail::ceil_log2(7u) << '\n'; // 3u
    seqan3::debug_stream << "ceil_log2(2^3 + 0): " << seqan3::detail::ceil_log2(8u) << '\n'; // 3u
    seqan3::debug_stream << "ceil_log2(2^3 + 1): " << seqan3::detail::ceil_log2(9u) << '\n'; // 4u

    return 0;
}
