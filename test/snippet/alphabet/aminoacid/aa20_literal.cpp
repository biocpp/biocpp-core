#include <bio/alphabet/aminoacid/aa20.hpp>

int main()
{
    using bio::operator""_aa20;
    
    bio::aa20_vector foo{"ABFUYR"_aa20};
    bio::aa20_vector bar = "ABFUYR"_aa20;
    auto bax = "ABFUYR"_aa20;
}
