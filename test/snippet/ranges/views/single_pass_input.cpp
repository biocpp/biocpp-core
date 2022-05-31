#include <string>

#include <bio/meta/debug_stream.hpp>
#include <bio/ranges/views/single_pass_input.hpp>

int main()
{
    std::string str{"hello"};
    auto v = str | seqan3::views::single_pass_input;
    auto b = v.begin();

    seqan3::debug_stream << *b << '\n';      // prints 'h'
    seqan3::debug_stream << *(++b) << '\n';  // prints 'e'
    seqan3::debug_stream << *(++b) << '\n';  // prints 'l'
    seqan3::debug_stream << *(++b) << '\n';  // prints 'l'
    seqan3::debug_stream << *(++b) << '\n';  // prints 'o'
}
