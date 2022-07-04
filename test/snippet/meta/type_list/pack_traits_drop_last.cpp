#include <bio/meta/type_list/traits.hpp>

int main()
{
    // Drop the last two types in the pack.
    static_assert(std::same_as<bio::meta::type_list<int, float>, bio::meta::detail::pack_traits::drop_last<2, int, float, bool, int>>);
}
