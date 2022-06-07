#include <vector>

#include <bio/alphabet/structure/dssp9.hpp>
#include <bio/alphabet/fmt.hpp>

int main()
{
    using bio::operator""_dssp9;

    // create vector
    std::vector<bio::dssp9> vec{'E'_dssp9, 'H'_dssp9, 'H'_dssp9, 'H'_dssp9, 'T'_dssp9, 'G'_dssp9};

    // modify and print
    vec[1] = 'C'_dssp9;

    for (bio::dssp9 chr : vec)
        fmt::print("{}", bio::to_char(chr));  // ECHHTG

    fmt::print("\n");
}
