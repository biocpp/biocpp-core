#include <string>
#include <vector>

#include <bio/meta/debug_stream.hpp>
#include <bio/ranges/views/type_reduce.hpp>

int main()
{
    std::string const vec{"foobar"};
    auto v = vec | bio::views::type_reduce;          // pipe notation; v is of type std::string_view
    bio::debug_stream << v << '\n';                  // "foobar"

    std::vector vec2{1, 2, 3};
    auto v2 = bio::views::type_reduce(vec2);         // functional notation; v2 is of type std::span
    bio::debug_stream << v2 << '\n';                 // "[1, 2, 3]"
}
