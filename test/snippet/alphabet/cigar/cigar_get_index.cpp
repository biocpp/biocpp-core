#include <bio/alphabet/cigar/cigar.hpp>
#include <bio/alphabet/cigar/cigar_op.hpp>
#include <bio/meta/debug_stream.hpp>

int main()
{
    using seqan3::get;
    using seqan3::operator""_cigar_op;

    seqan3::cigar letter{10, 'M'_cigar_op};

    uint32_t size{get<0>(letter)};               // Note this is equivalent to get<uint32_t>(letter)
    seqan3::cigar_op cigar_char{get<1>(letter)}; // Note this is equivalent to get<seqan3::cigar_op>(letter)

    seqan3::debug_stream << "Size is "       << size       << '\n';
    seqan3::debug_stream << "Cigar char is " << cigar_char << '\n'; // seqan3::debug_stream converts to char on the fly.

}
