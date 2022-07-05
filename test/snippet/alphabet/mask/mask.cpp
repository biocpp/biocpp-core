#include <bio/alphabet/mask/mask.hpp>
#include <bio/alphabet/fmt.hpp>

int main()
{
    bio::alphabet::mask my_mask = bio::alphabet::mask::MASKED;
    bio::alphabet::mask another_mask{};

    my_mask.assign_rank(false);  // will assign my_mask the value mask::UNMASKED
    another_mask.assign_rank(0); // will also assign another_mask the value mask::UNMASKED

    if (my_mask.to_rank() == another_mask.to_rank())
        fmt::print("Both are UNMASKED!\n");
}
