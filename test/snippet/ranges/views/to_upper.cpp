#include <string>
#include <vector>

#include <bio/alphabet/fmt.hpp>
#include <bio/ranges/views/to_upper.hpp>

int main()
{
    std::string s{"CHanGED!"};
    std::vector<std::string> sv{"changed", "UNCHANGED!"};
    auto v1 = s | bio::views::to_upper;
    auto v2 = sv | bio::views::to_upper;

    fmt::print("{}\n", v1); // => "CHANGED!"
    fmt::print("{}\n", v2); // => ["CHANGED", "UNCHANGED!"]
}
