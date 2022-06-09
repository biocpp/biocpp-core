#include <ranges>                               // include all of the standard library's views

#include <sharg/all.hpp>                        // Optional: the sharg-parser

#include <bio/alphabet/fmt.hpp>
#include <bio/ranges/views/all.hpp>            // include all of BioCpp's views


int main(int argc, char** argv)
{
    // We use the sharg::argument_parser
    sharg::argument_parser myparser{"Assignment-3", argc, argv}; // initialize
    std::string s{};

    myparser.add_positional_option(s, "Please specify the DNA string.");

    try
    {
       myparser.parse();
    }
    catch (sharg::argument_parser_error const & ext)             // the user did something wrong
    {
       fmt::print(stderr, "[PARSER ERROR] {}\n", ext.what()); // you can customize your error message
       return 0;
    }

    auto s_as_dna = s | bio::views::char_to<bio::dna5>;
    // Bonus:
    //auto s_as_dna = s | std::views::transform([] (char const c)
    //{
    //    return bio::assign_char_strictly_to(c, bio::dna5{});
    //});

    fmt::print("Original: {}\n", s_as_dna);
    fmt::print("RevComp:  {}\n", s_as_dna | std::views::reverse | bio::views::complement);
    fmt::print("Frames:   {}\n", s_as_dna | bio::views::translate);
}
