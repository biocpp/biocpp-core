#include <string>
#include <vector>

#include <bio/alphabet/fmt.hpp>
#include <bio/ranges/views/to_lower.hpp>

int main()
{
    std::string s{"CHanGED!"};
    std::vector<std::string> sv{"CHANGED", "unchanged!"};
    auto v1 = s | bio::views::to_lower;
    auto v2 = sv | bio::views::to_lower;

    fmt::print("{}\n", v1); // => "changed!"
    fmt::print("{}\n", v2); // => ["changed", "unchanged!"]
}
