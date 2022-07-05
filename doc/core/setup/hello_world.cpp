#include <bio/alphabet/aminoacid/all.hpp>
#include <bio/alphabet/fmt.hpp>

using bio::alphabet::operator""_aa27;

int main()
{
    std::vector<bio::alphabet::aa27> protein = "HELLOWORLD"_aa27;
    fmt::print("{}\n", protein);
    return 0;
}
