#include <string_view>
#include <vector>

#include <bio/alphabet/fmt.hpp>
#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/ranges/container/bitcompressed_vector.hpp>      // include bitcompressed vector

using namespace bio::alphabet::literals;

/* call with two arguments:
 * SIZE
 * "0" or "1" for vector/bitvector
 */
int main(int argc, char ** argv)
{
    if (argc != 3)
    {
        // add error handling here
        return 0;
    }
    size_t size = std::stoull(argv[1]);
    bool use_bitvector = (std::string_view{argv[2]} == "1");

    if (use_bitvector)
    {
        bio::ranges::bitcompressed_vector<bio::alphabet::dna4> vector;
        vector.resize(size, 'A'_dna4);
        fmt::print("Allocated bio::ranges::bitcompressed_vector<bio::alphabet::dna4> of size {}\n", vector.size());
    }
    else
    {
        std::vector<bio::alphabet::dna4> vector{size};
        fmt::print("Allocated std::vector<bio::alphabet::dna4> of size {}\n", vector.size());
    }
}
