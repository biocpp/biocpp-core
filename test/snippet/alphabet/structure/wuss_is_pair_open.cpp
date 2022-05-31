#include <bio/alphabet/structure/wuss.hpp>

int main()
{
    using bio::operator""_wuss51;

    bool is_opening_char = '{'_wuss51.is_pair_open();                // true
    bool is_opening_char_alt = bio::is_pair_open('.'_wuss51);     // false
}
