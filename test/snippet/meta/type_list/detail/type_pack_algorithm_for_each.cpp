#include <string>

#include <bio/meta/debug_stream.hpp>
#include <bio/meta/type_pack/detail/type_pack_algorithm.hpp>

int main()
{
    auto fn = [](auto && a)
    {
        seqan3::debug_stream << a;
    };

    // prints each argument, i.e. "0, 1, 2, 3\n"
    seqan3::detail::for_each(fn, 0, ", ", 1.0, ", ", std::string{"2, 3"}, '\n');

    // is the same as explicitly writing
    fn(0);
    fn(", ");
    fn(1.0);
    fn(", ");
    fn(std::string{"2, 3"});
    fn('\n');
    return 0;
}
