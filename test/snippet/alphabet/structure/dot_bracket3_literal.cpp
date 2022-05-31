#include <vector>

#include <bio/alphabet/structure/dot_bracket3.hpp>

int main()
{
    using bio::operator""_db3;

    // Using the string literal to assign a vector of dot brackets:
    std::vector<bio::dot_bracket3> foo{".(..)."_db3};
    std::vector<bio::dot_bracket3> bar = ".(..)."_db3;
    auto bax = ".(..)."_db3;
}
