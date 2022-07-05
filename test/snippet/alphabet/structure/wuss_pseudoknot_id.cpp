#include <bio/alphabet/structure/wuss.hpp>
#include <bio/alphabet/fmt.hpp>

int main()
{
    using bio::alphabet::operator""_wuss51;

    auto pk_opt = '.'_wuss51.pseudoknot_id();                    // std::optional -> false
    pk_opt = bio::alphabet::pseudoknot_id('{'_wuss51);                     // std::optional -> true: 3

    if (pk_opt)
        fmt::print("{}", *pk_opt);                               // 3
}
