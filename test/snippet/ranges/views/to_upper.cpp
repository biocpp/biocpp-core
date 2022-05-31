#include <string>
#include <vector>

#include <bio/meta/debug_stream.hpp>
#include <bio/ranges/views/to_upper.hpp>

int main()
{
    std::string s{"CHanGED!"};
    std::vector<std::string> sv{"changed", "UNCHANGED!"};
    auto v1 = s | bio::views::to_upper;
    auto v2 = sv | bio::views::to_upper;

    bio::debug_stream << v1 << '\n'; // => "CHANGED!"
    bio::debug_stream << v2 << '\n'; // => ["CHANGED", "UNCHANGED!"]
}
