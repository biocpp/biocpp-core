#include <string>

#include <bio/meta/debug_stream.hpp>
#include <bio/ranges/views/single_pass_input.hpp>

int main()
{
    std::string str{"hello"};
    auto v = str | bio::views::single_pass_input;
    auto b = v.begin();

    bio::debug_stream << *b << '\n';      // prints 'h'
    bio::debug_stream << *(++b) << '\n';  // prints 'e'
    bio::debug_stream << *(++b) << '\n';  // prints 'l'
    bio::debug_stream << *(++b) << '\n';  // prints 'l'
    bio::debug_stream << *(++b) << '\n';  // prints 'o'
}
