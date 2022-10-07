#include <bio/alphabet/aminoacid/aa20.hpp>

int main()
{
    using namespace bio::alphabet::literals;

    std::vector<bio::alphabet::aa20> bar = "ADFCYR"_aa20;
    auto bax = "ADFCYR"_aa20;
}
