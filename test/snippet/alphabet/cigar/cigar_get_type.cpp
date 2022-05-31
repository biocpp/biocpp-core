#include <bio/alphabet/cigar/cigar.hpp>
#include <bio/alphabet/cigar/cigar_op.hpp>
#include <bio/meta/debug_stream.hpp>

int main()
{
    using bio::get;
    using bio::operator""_cigar_op;

    bio::cigar letter{10, 'M'_cigar_op};

    uint32_t size{get<uint32_t>(letter)};                       // Note this is equivalent to get<0>(letter)
    bio::cigar_op cigar_char{get<bio::cigar_op>(letter)}; // Note this is equivalent to get<1>(letter)

    bio::debug_stream << "Size is "       << size       << '\n';
    bio::debug_stream << "Cigar char is " << cigar_char << '\n'; // bio::debug_stream converts to char on the fly.

}
