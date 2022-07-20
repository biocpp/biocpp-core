#include <bio/alphabet/aminoacid/aa20.hpp>

int main()
{
    using namespace bio::alphabet::literals;

    bio::alphabet::aa20_vector foo{"ABFUYR"_aa20};
    bio::alphabet::aa20_vector bar = "ABFUYR"_aa20;
    auto bax = "ABFUYR"_aa20;
}
