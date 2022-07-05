#include <bio/alphabet/structure/wuss.hpp>
#include <bio/alphabet/fmt.hpp>

int main()
{
    using bio::alphabet::operator""_wuss51;

    // create vector
    std::vector<bio::alphabet::wuss51> vec{'.'_wuss51, '>'_wuss51, '>'_wuss51};
    // modify and print
    vec[1] = '<'_wuss51;
    for (bio::alphabet::wuss51 chr : vec)
        fmt::print("{}", bio::alphabet::to_char(chr));  // .<>
    fmt::print("\n");
}
