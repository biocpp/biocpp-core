#include <bio/meta/type_list/traits.hpp>

int main()
{
    // Drop the first two types in the pack.
    static_assert(std::same_as<bio::type_list<bool, int>, bio::pack_traits::drop<2, float, double, bool, int>>);
}
