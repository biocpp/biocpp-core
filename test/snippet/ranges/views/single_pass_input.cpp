#include <string>

#include <bio/alphabet/fmt.hpp>
#include <bio/ranges/views/single_pass_input.hpp>

int main()
{
    std::string str{"hello"};
    auto v = str | bio::views::single_pass_input;
    auto b = v.begin();

    fmt::print("{}\n", *b);      // prints 'h'
    fmt::print("{}\n", *(++b));  // prints 'e'
    fmt::print("{}\n", *(++b));  // prints 'l'
    fmt::print("{}\n", *(++b));  // prints 'l'
    fmt::print("{}\n", *(++b));  // prints 'o'
}
