#include <bio/alphabet/aminoacid/aa20.hpp>

int main()
{
    using bio::operator""_aa20;

    bio::aa20 acid1{'A'_aa20};
    
    auto acid2 = 'Y'_aa20; // type = aa20
}
