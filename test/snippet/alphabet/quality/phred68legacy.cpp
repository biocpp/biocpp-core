#include <bio/alphabet/quality/phred68legacy.hpp>
#include <bio/meta/debug_stream.hpp>

int main()
{
    bio::phred68legacy phred;
    phred.assign_phred(-2);
    bio::debug_stream << (int) phred.to_phred() << "\n"; // -2
    bio::debug_stream << phred.to_char() << "\n";        // '>'
    bio::debug_stream << (int) phred.to_rank() << "\n";  // 3
}
