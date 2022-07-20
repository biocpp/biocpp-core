#include <bio/alphabet/aminoacid/aa27.hpp>

int main()
{
    using namespace bio::alphabet::literals;

    bio::alphabet::aa27 acid1{'A'_aa27};

    auto acid2 = 'Y'_aa27; // type = aa27
}
