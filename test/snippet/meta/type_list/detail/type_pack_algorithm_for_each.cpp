#include <string>

#include <bio/alphabet/fmt.hpp>
#include <bio/meta/type_pack/detail/type_pack_algorithm.hpp>

int main()
{
    auto fn = [](auto && a)
    {
        fmt::print("{}", a);
    };

    // prints each argument, i.e. "0, 1, 2, 3\n"
    bio::detail::for_each(fn, 0, ", ", 1.0, ", ", std::string{"2, 3"}, '\n');

    // is the same as explicitly writing
    fn(0);
    fn(", ");
    fn(1.0);
    fn(", ");
    fn(std::string{"2, 3"});
    fn('\n');
    return 0;
}
