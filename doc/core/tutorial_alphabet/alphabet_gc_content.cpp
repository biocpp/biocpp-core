//! [exercise]
#include <array>     // std::array
#include <string>    // std::string
#include <vector>    // std::vector

#include <sharg/all.hpp>

#include <bio/alphabet/all.hpp>
#include <bio/alphabet/fmt.hpp>
#include <bio/ranges/views/all.hpp>       // optional: use views to convert the input string to a dna5 sequence

using bio::alphabet::operator""_dna5;

int main (int argc, char * argv[])
{
    std::string input{};
    sharg::argument_parser parser("GC-Content", argc, argv);
    parser.add_positional_option(input, "Specify an input sequence.");
    try
    {
        parser.parse();
    }
    catch (sharg::argument_parser_error const & ext) // the input is invalid
    {
        fmt::print(stderr, "[PARSER ERROR] {}\n", ext.what());
        return 0;
    }

    // Option1: Convert the input to a dna5 sequence by copy-conversion
    std::vector<bio::alphabet::dna5> sequence;
    sequence.resize(input.size());
    for (size_t i = 0; i < input.size(); ++i)
        bio::alphabet::assign_char_to(input[i], sequence[i]);

    // Option1: use views for the conversion. Views will be introduced in the next chapter.
    //auto sequence = input | bio::views::char_to<bio::alphabet::dna5>;

    // Initialise an array with count values for dna5 symbols.
    std::array<size_t, bio::alphabet::dna5::alphabet_size> count{}; // default initialised with zeroes

    // Increase the symbol count according to the sequence.
    for (bio::alphabet::dna5 symbol : sequence)
        ++count[symbol.to_rank()];

    // Calculate the GC content: (#G + #C) / (#A + #T + #G + #C).
    size_t gc = count['C'_dna5.to_rank()] + count['G'_dna5.to_rank()];
    size_t atgc = input.size() - count['N'_dna5.to_rank()];
    float gc_content = 1.0f * gc / atgc;

    fmt::print("The GC content of {} is {}%\n", sequence, 100 * gc_content);

    return 0;
}
//! [exercise]
