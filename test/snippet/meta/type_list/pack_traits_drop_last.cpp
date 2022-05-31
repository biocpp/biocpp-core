#include <bio/meta/type_list/traits.hpp>

int main()
{
    // Drop the last two types in the pack.
    static_assert(std::same_as<bio::type_list<int, float>, bio::pack_traits::drop_last<2, int, float, bool, int>>);
}
