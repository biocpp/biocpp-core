#include <bio/alphabet/quality/phred63.hpp>
#include <bio/alphabet/fmt.hpp>

int main()
{
    bio::alphabet::phred63 phred;
    phred.assign_rank(2); // wrapper for assign_phred(2)
    fmt::print("{}\n", static_cast<int>(phred.to_phred())); // 2
    fmt::print("{}\n", phred.to_char());                    // '#'
    fmt::print("{}\n", static_cast<int>(phred.to_rank()));  // 2

    bio::alphabet::phred63 another_phred{49};
    fmt::print("{}\n", another_phred.to_phred());           // 49
    // we need to cast to (int) for human readable console output
}
