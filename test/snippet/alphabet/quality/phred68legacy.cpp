#include <bio/alphabet/quality/phred68legacy.hpp>
#include <bio/alphabet/fmt.hpp>

int main()
{
    bio::phred68legacy phred;
    phred.assign_phred(-2);
    fmt::print("{}\n", (int) phred.to_phred()); // -2
    fmt::print("{}\n", phred.to_char());        // '>'
    fmt::print("{}\n", (int) phred.to_rank());  // 3
}
