#include <string>
#include <vector>

#include <bio/meta/debug_stream.hpp>
#include <bio/ranges/views/to_lower.hpp>

int main()
{
    std::string s{"CHanGED!"};
    std::vector<std::string> sv{"CHANGED", "unchanged!"};
    auto v1 = s | seqan3::views::to_lower;
    auto v2 = sv | seqan3::views::to_lower;

    seqan3::debug_stream << v1 << '\n'; // => "changed!"
    seqan3::debug_stream << v2 << '\n'; // => ["changed", "unchanged!"]
}
