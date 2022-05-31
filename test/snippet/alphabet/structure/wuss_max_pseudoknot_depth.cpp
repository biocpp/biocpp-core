#include <bio/alphabet/structure/wuss.hpp>

int main()
{
    uint8_t max_depth = bio::wuss51::max_pseudoknot_depth;             // 22
    uint8_t max_depth_alt = bio::max_pseudoknot_depth<bio::wuss51>; // 22
}
