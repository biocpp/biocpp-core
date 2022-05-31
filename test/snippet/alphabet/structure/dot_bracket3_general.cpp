#include <vector>

#include <bio/alphabet/structure/dot_bracket3.hpp>
#include <bio/meta/debug_stream.hpp>

int main()
{
    using bio::operator""_db3;

    // create vector
    std::vector<bio::dot_bracket3> vec{'.'_db3, ')'_db3, ')'_db3};

    // modify and print
    vec[1] = '('_db3;

    for (bio::dot_bracket3 chr : vec)
        bio::debug_stream << bio::to_char(chr);  // .()

    bio::debug_stream << "\n";
}
