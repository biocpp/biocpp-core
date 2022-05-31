#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/meta/char_operations/predicate.hpp>

int main()
{
//! [is_eof]
bio::is_eof(EOF);  // returns true
bio::is_eof('C');  // returns false
//! [is_eof]

//! [is_cntrl]
bio::is_cntrl('\0');  // returns true.
//! [is_cntrl]

//! [is_print]
bio::is_print(' ');  // returns true.
//! [is_print]

//! [is_space]
bio::is_space('\n');  // returns true.
//! [is_space]

//! [is_blank]
bio::is_blank('\t');  // returns true.
//! [is_blank]

//! [is_graph]
bio::is_graph('%');  // returns true.
//! [is_graph]

//! [is_punct]
bio::is_punct(':');  // returns true.
//! [is_punct]

//! [is_alnum]
bio::is_alnum('9');  // returns true.
//! [is_alnum]

//! [is_alpha]
bio::is_alpha('z');  // returns true.
//! [is_alpha]

//! [is_upper]
bio::is_upper('K');  // returns true.
//! [is_upper]

//! [is_lower]
bio::is_lower('a');  // returns true.
//! [is_lower]

//! [is_digit]
bio::is_digit('1');  // returns true.
//! [is_digit]

//! [is_xdigit]
bio::is_xdigit('e');  // returns true.
//! [is_xdigit]
}
