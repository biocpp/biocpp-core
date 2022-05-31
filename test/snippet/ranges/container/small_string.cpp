#include <bio/meta/debug_stream.hpp>
#include <bio/ranges/container/small_string.hpp>

int main()
{
    constexpr bio::small_string sm{"hello"};     // construct from string literal at compile time!

    static_assert(sm[0] == 'h');                    // This way I can also test it at compile-time

    bio::debug_stream << sm.size() << '\n'; // prints 5! (the null character is only stored internally)

    // conversion to a normal string:
    std::string sm_string{sm.str()};
    // access data directly with a pointer to the underlying zero-terminated array:
    char const * sm_cstr{sm.c_str()};

    bio::debug_stream << sm << sm_string << sm_cstr << '\n'; // prints "hellohellohello"
}
