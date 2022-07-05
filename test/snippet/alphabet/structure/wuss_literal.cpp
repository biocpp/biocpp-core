#include <bio/alphabet/structure/wuss.hpp>

int main()
{
    using bio::alphabet::operator""_wuss51;

    // Using the string literal to assign a vector of WUSS characters:
    std::vector<bio::alphabet::wuss<>> foo{".<..>."_wuss51};
    std::vector<bio::alphabet::wuss<>> bar = ".<..>."_wuss51;
    auto bax = ".<..>."_wuss51;
}
