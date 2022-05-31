#include <vector>

#include <sharg/all.hpp>                                        // Optional: the sharg-parser

#include <bio/meta/debug_stream.hpp>
#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/ranges/container/bitcompressed_vector.hpp>      // include bitcompressed vector

using bio::operator""_dna4;

int main(int argc, char ** argv)
{
    sharg::argument_parser myparser("Vector-implementations-comparison", argc, argv);
    size_t size{};
    bool use_bitvector{};
    myparser.add_positional_option(size, "Size of vector");
    myparser.add_flag(use_bitvector, 'b', "bitvector", "Use bitvector instead of vector");

    try
    {
         myparser.parse();
    }
    catch (sharg::argument_parser_error const & ext)                     // catch user errors
    {
        std::cerr << "[Error] " << ext.what() << "\n";
        return -1;
    }

    if (use_bitvector)
    {
        bio::bitcompressed_vector<bio::dna4> vector;
        vector.resize(size, 'A'_dna4);
        bio::debug_stream << "Allocated bio::bitcompressed_vector<bio::dna4> of size "
                             << vector.size() << '\n';
    }
    else
    {
        std::vector<bio::dna4> vector{size};
        bio::debug_stream << "Allocated std::vector<bio::dna4> of size " << vector.size() << '\n';
    }
}
