#include <bio/alphabet/quality/phred42.hpp>
#include <bio/alphabet/fmt.hpp>

int main()
{
    bio::alphabet::phred42 phred;
    phred.assign_rank(2); // wrapper for assign_phred(2)
    fmt::print("{}\n", (int) phred.to_phred()); // 2
    fmt::print("{}\n", phred.to_char());        // '#'
    fmt::print("{}\n", (int) phred.to_rank());  // 2

    bio::alphabet::phred42 another_phred;
    another_phred.assign_phred(49); // converted down to 41
    fmt::print("{}\n", another_phred.to_phred()); // 41
    // we need to cast to(int)for human readable console output
}
