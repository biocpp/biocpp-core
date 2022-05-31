#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/meta/debug_stream.hpp>

int main()
{
    using bio::operator""_dna4;

    bio::dna4 my_letter{'C'_dna4};

    my_letter.assign_char('F'); // characters other than IUPAC characters are implicitly converted to A.
    bio::debug_stream << my_letter; // prints "F"

    // IUPAC characters are implicitly converted to their best fitting representative
    bio::debug_stream << my_letter.assign_char('R'); // prints "A"
    bio::debug_stream << my_letter.assign_char('Y'); // prints "C"
    bio::debug_stream << my_letter.assign_char('S'); // prints "C"
    bio::debug_stream << my_letter.assign_char('W'); // prints "A"
    bio::debug_stream << my_letter.assign_char('K'); // prints "G"
    bio::debug_stream << my_letter.assign_char('M'); // prints "A"
    bio::debug_stream << my_letter.assign_char('B'); // prints "C"
    bio::debug_stream << my_letter.assign_char('D'); // prints "A"
    bio::debug_stream << my_letter.assign_char('H'); // prints "A"
    bio::debug_stream << my_letter.assign_char('V'); // prints "A"

    my_letter.assign_char('a'); // lower case letters are the same as their upper case equivalent
    bio::debug_stream << my_letter; // prints "A";
}
