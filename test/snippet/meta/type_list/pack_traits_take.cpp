#include <bio/meta/type_list/traits.hpp>

int main()
{

    // Take the first two types in the pack.
    static_assert(std::same_as<bio::meta::type_list<int, float>, bio::meta::detail::pack_traits::take<2, int, float, bool, double>>);
}
