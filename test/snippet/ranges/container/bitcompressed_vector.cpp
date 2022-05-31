#include <vector>

#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/ranges/container/bitcompressed_vector.hpp>

int main()
{
    using bio::operator""_dna4;

    std::vector<bio::dna4>                  v0{"ACGT"_dna4}; // data occupies 4 bytes in memory
    bio::bitcompressed_vector<bio::dna4> v1{"ACGT"_dna4}; // data occupies 1 byte in memory
}
