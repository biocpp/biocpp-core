#include <vector>

#include <bio/alphabet/structure/dot_bracket3.hpp>
#include <bio/alphabet/fmt.hpp>

int main()
{
    using bio::alphabet::operator""_db3;

    // create vector
    std::vector<bio::alphabet::dot_bracket3> vec{'.'_db3, ')'_db3, ')'_db3};

    // modify and print
    vec[1] = '('_db3;

    for (bio::alphabet::dot_bracket3 chr : vec)
        fmt::print("{}", bio::alphabet::to_char(chr));  // .()

    fmt::print("\n");
}
