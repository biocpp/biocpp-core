#include <bio/meta/type_list/traits.hpp>

int main()
{
    // Get the size of the pack.
    static_assert(bio::detail::pack_traits::size<int, float, bool, int> == 4);
}
