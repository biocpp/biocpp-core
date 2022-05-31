#include <sstream>

#include <bio/alphabet/nucleotide/dna5.hpp>
#include <bio/meta/debug_stream.hpp>

int main()
{
    using bio::operator""_dna5;

    std::ostringstream o;
    bio::debug_stream_type my_stream{o};

    my_stream << "ACGT"_dna5;

    o.flush();
    bio::debug_stream << o.str(); // prints the string stream's buffer: "ACGT"
}
