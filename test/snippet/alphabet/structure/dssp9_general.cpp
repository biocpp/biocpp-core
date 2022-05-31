#include <vector>

#include <bio/alphabet/structure/dssp9.hpp>
#include <bio/meta/debug_stream.hpp>

int main()
{
    using bio::operator""_dssp9;

    // create vector
    std::vector<bio::dssp9> vec{'E'_dssp9, 'H'_dssp9, 'H'_dssp9, 'H'_dssp9, 'T'_dssp9, 'G'_dssp9};

    // modify and print
    vec[1] = 'C'_dssp9;

    for (bio::dssp9 chr : vec)
        bio::debug_stream << bio::to_char(chr);  // ECHHTG
        
    bio::debug_stream << "\n";
}
