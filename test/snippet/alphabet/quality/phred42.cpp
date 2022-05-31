#include <bio/alphabet/quality/phred42.hpp>
#include <bio/meta/debug_stream.hpp>

int main()
{
    bio::phred42 phred;
    phred.assign_rank(2); // wrapper for assign_phred(2)
    bio::debug_stream << (int) phred.to_phred() << "\n"; // 2
    bio::debug_stream << phred.to_char() << "\n";        // '#'
    bio::debug_stream << (int) phred.to_rank() << "\n";  // 2

    bio::phred42 another_phred;
    another_phred.assign_phred(49); // converted down to 41
    bio::debug_stream << another_phred.to_phred() << "\n"; // 41
    // we need to cast to(int)for human readable console output
}
