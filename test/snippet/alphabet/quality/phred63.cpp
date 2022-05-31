#include <bio/alphabet/quality/phred63.hpp>
#include <bio/meta/debug_stream.hpp>

int main()
{
    bio::phred63 phred;
    phred.assign_rank(2); // wrapper for assign_phred(2)
    bio::debug_stream << static_cast<int>(phred.to_phred()) << "\n"; // 2
    bio::debug_stream << phred.to_char() << "\n";        // '#'
    bio::debug_stream << static_cast<int>(phred.to_rank()) << "\n";  // 2

    bio::phred63 another_phred{49};
    bio::debug_stream << another_phred.to_phred() << "\n"; // 49
    // we need to cast to (int) for human readable console output
}
