#include <bio/meta/debug_stream.hpp>

int main()
{
    uint8_t i = 71;
    bio::debug_stream << '\'' << i << "'\n";                                           // prints '71' (because flag is set by default)
    bio::debug_stream.unsetf(bio::fmtflags2::small_int_as_number);                  // unsets the flag
    bio::debug_stream << '\'' << i << "'\n";                                           // prints 'G'
    bio::debug_stream << bio::fmtflags2::small_int_as_number << '\'' << i << "'\n"; // prints '71' again
    // instead of formatting the stream "inline", one can also call .setf()
}
