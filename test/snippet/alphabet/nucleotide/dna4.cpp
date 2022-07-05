#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/alphabet/fmt.hpp>

int main()
{
    using bio::alphabet::operator""_dna4;

    bio::alphabet::dna4 my_letter{'C'_dna4};

    my_letter.assign_char('F'); // characters other than IUPAC characters are implicitly converted to A.
    fmt::print("{}", my_letter); // prints "F"

    // IUPAC characters are implicitly converted to their best fitting representative
    fmt::print("{}", my_letter.assign_char('R')); // prints "A"
    fmt::print("{}", my_letter.assign_char('Y')); // prints "C"
    fmt::print("{}", my_letter.assign_char('S')); // prints "C"
    fmt::print("{}", my_letter.assign_char('W')); // prints "A"
    fmt::print("{}", my_letter.assign_char('K')); // prints "G"
    fmt::print("{}", my_letter.assign_char('M')); // prints "A"
    fmt::print("{}", my_letter.assign_char('B')); // prints "C"
    fmt::print("{}", my_letter.assign_char('D')); // prints "A"
    fmt::print("{}", my_letter.assign_char('H')); // prints "A"
    fmt::print("{}", my_letter.assign_char('V')); // prints "A"

    my_letter.assign_char('a'); // lower case letters are the same as their upper case equivalent
    fmt::print("{}", my_letter); // prints "A";
}
