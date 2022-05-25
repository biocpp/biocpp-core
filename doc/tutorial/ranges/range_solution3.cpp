#include <ranges>                               // include all of the standard library's views

#include <sharg/all.hpp>                        // Optional: the sharg-parser

#include <seqan3/core/debug_stream.hpp>
#include <seqan3/range/views/all.hpp>           // include all of SeqAn's views


int main(int argc, char** argv)
{
    // We use the sharg_::argument_parser
    sharg::argument_parser myparser{"Assignment-3", argc, argv}; // initialize
    std::string s{};

    myparser.add_positional_option(s, "Please specify the DNA string.");

    try
    {
       myparser.parse();
    }
    catch (sharg::argument_parser_error const & ext) // the user did something wrong
    {
       std::cerr << "[PARSER ERROR]" << ext.what() << '\n'; // you can customize your error message
       return 0;
    }

    auto s_as_dna = s | seqan3::views::char_to<seqan3::dna5>;
    // Bonus:
    //auto s_as_dna = s | std::views::transform([] (char const c)
    //{
    //    return seqan3::assign_char_strictly_to(c, seqan3::dna5{});
    //});

    seqan3::debug_stream << "Original: " << s_as_dna << '\n';
    seqan3::debug_stream << "RevComp:  " << (s_as_dna | std::views::reverse | seqan3::views::complement) << '\n';
    seqan3::debug_stream << "Frames:   " << (s_as_dna | seqan3::views::translate) << '\n';
}
