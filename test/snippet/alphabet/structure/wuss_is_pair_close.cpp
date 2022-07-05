#include <bio/alphabet/structure/wuss.hpp>

int main()
{
    using bio::alphabet::operator""_wuss51;

    bool is_closing_char = '}'_wuss51.is_pair_close();               // true
    bool is_closing_char_alt = bio::alphabet::is_pair_close('.'_wuss51);    // false
}
