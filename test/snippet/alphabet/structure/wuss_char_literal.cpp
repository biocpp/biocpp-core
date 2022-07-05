#include <bio/alphabet/structure/wuss.hpp>

int main()
{
    using bio::alphabet::operator""_wuss51;

    // Using the char literal to assign a single WUSS character:
    bio::alphabet::wuss51 my_letter{'~'_wuss51};

    my_letter.assign_char('<'); // <- assigns the char explicitly
}
