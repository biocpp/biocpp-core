#include <bio/alphabet/structure/wuss.hpp>

int main()
{
    using bio::alphabet::operator""_wuss51;

    bool is_unpaired_char = '.'_wuss51.is_unpaired();                // true
    bool is_unpaired_char_alt = bio::alphabet::is_unpaired('{'_wuss51);     // false
}
