#include <string>
#include <vector>

#include <bio/alphabet/fmt.hpp>
#include <bio/ranges/views/type_reduce.hpp>

int main()
{
    std::string const vec{"foobar"};
    auto v = vec | bio::views::type_reduce;          // pipe notation; v is of type std::string_view
    fmt::print("{}\n", v);                  // "foobar"

    std::vector vec2{1, 2, 3};
    auto v2 = bio::views::type_reduce(vec2);         // functional notation; v2 is of type std::span
    fmt::print("{}\n", v2);                 // "[1, 2, 3]"
}
