#include <bio/meta/type_list/traits.hpp>

int main()
{
    // Count the number of type int in the pack.
    static_assert(bio::pack_traits::count<int, int, float, bool, int> == 2);
}
