#include <bio/alphabet/structure/wuss.hpp>

int main()
{
    using bio::operator""_wuss51;

    // Using the string literal to assign a vector of WUSS characters:
    std::vector<bio::wuss<>> foo{".<..>."_wuss51};
    std::vector<bio::wuss<>> bar = ".<..>."_wuss51;
    auto bax = ".<..>."_wuss51;
}
