#include <bio/meta/type_list/traits.hpp>

int main()
{
    // Drop the first two types in the pack.
    static_assert(std::same_as<bio::meta::type_list<bool, int>, bio::meta::detail::pack_traits::drop<2, float, double, bool, int>>);
}
