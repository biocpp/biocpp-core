#include <string>
#include <vector>

#include <bio/ranges/type_traits.hpp>

int main()
{
    // these evaluate to true:
    static_assert(bio::ranges::range_compatible<std::string,              std::vector<char>>);
    static_assert(bio::ranges::range_compatible<std::vector<std::string>, std::vector<std::vector<char>>>);
}
