#include <vector>

#include <bio/alphabet/structure/dssp9.hpp>

int main()
{
    using bio::alphabet::operator""_dssp9;

    // Using the string literal to assign a vector of DSSP annotations:
    std::vector<bio::alphabet::dssp9> foo{"EHHHHT"_dssp9};
    std::vector<bio::alphabet::dssp9> bar = "EHHHHT"_dssp9;
    auto bax = "EHHHHT"_dssp9;
}
