#include <ranges>                               // include all of the standard library's views

#include <sharg/all.hpp>                        // Optional: the sharg-parser

#include <bio/meta/debug_stream.hpp>
#include <bio/ranges/views/all.hpp>           // include all of SeqAn's views


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

    auto s_as_dna = s | bio::views::char_to<bio::dna5>;
    // Bonus:
    //auto s_as_dna = s | std::views::transform([] (char const c)
    //{
    //    return bio::assign_char_strictly_to(c, bio::dna5{});
    //});

    bio::debug_stream << "Original: " << s_as_dna << '\n';
    bio::debug_stream << "RevComp:  " << (s_as_dna | std::views::reverse | bio::views::complement) << '\n';
    bio::debug_stream << "Frames:   " << (s_as_dna | bio::views::translate) << '\n';
}
