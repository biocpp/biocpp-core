#include <bio/alphabet/aminoacid/aa27.hpp>

int main()
{
    using bio::alphabet::operator""_aa27;

    bio::alphabet::aa27_vector foo{"ABFUYR"_aa27};
    bio::alphabet::aa27_vector bar = "ABFUYR"_aa27;
    auto bax = "ABFUYR"_aa27;
}
