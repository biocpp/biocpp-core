#include <bio/alphabet/cigar/cigar.hpp>
#include <bio/alphabet/cigar/cigar_op.hpp>
#include <bio/alphabet/fmt.hpp>

int main()
{
    using bio::operator""_cigar_op;

    bio::cigar letter{10, 'M'_cigar_op};

    uint32_t size{get<0>(letter)};                // Note this is equivalent to get<uint32_t>(letter)
    bio::cigar_op cigar_char{get<1>(letter)};     // Note this is equivalent to get<bio::cigar_op>(letter)

    fmt::print("Size is {}\n", size);
    fmt::print("Cigar char is {}\n", cigar_char);

}
