#include <bio/alphabet/aminoacid/all.hpp>
#include <bio/alphabet/fmt.hpp>

using namespace bio::alphabet::literals;

int main()
{
    std::vector<bio::alphabet::aa27> protein = "HELLOWORLD"_aa27;
    fmt::print("{}\n", protein);
    return 0;
}
