#include <bio/meta/type_list/traits.hpp>

int main()
{
    // Take the last two types in the pack.
    static_assert(std::same_as<bio::meta::type_list<bool, int>, bio::meta::detail::pack_traits::take_last<2, int, float, bool, int>>);
}
