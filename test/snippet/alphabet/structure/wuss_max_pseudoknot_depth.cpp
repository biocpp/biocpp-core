#include <bio/alphabet/structure/wuss.hpp>

int main()
{
    uint8_t max_depth = bio::alphabet::wuss51::max_pseudoknot_depth;             // 22
    uint8_t max_depth_alt = bio::alphabet::max_pseudoknot_depth<bio::alphabet::wuss51>; // 22
}
