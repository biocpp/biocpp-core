#include <bio/alphabet/aminoacid/aa27.hpp>

int main()
{
    using namespace bio::alphabet::literals;

    std::vector<bio::alphabet::aa27> foo{"ABFUYR"_aa27};
    std::vector<bio::alphabet::aa27> bar = "ABFUYR"_aa27;
    auto bax = "ABFUYR"_aa27;
}
