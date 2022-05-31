#include <string>
#include <vector>

#include <bio/meta/debug_stream.hpp>
#include <bio/ranges/views/to_upper.hpp>

int main()
{
    std::string s{"CHanGED!"};
    std::vector<std::string> sv{"changed", "UNCHANGED!"};
    auto v1 = s | seqan3::views::to_upper;
    auto v2 = sv | seqan3::views::to_upper;

    seqan3::debug_stream << v1 << '\n'; // => "CHANGED!"
    seqan3::debug_stream << v2 << '\n'; // => ["CHANGED", "UNCHANGED!"]
}
