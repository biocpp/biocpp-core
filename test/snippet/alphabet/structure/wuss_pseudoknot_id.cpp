#include <bio/alphabet/structure/wuss.hpp>
#include <bio/meta/debug_stream.hpp>

int main()
{
    using bio::operator""_wuss51;

    auto pk_opt = '.'_wuss51.pseudoknot_id();                       // std::optional -> false
    pk_opt = bio::pseudoknot_id('{'_wuss51);                     // std::optional -> true: 3

    if (pk_opt)
        bio::debug_stream << *pk_opt;                            // 3
}
