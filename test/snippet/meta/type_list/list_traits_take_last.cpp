#include <bio/meta/type_list/traits.hpp>

int main()
{
    using list_t = bio::meta::type_list<int, float, bool, int>;

    // Take the last two types in list_t.
    static_assert(std::same_as<bio::meta::type_list<bool, int>, bio::meta::list_traits::take_last<2, list_t>>);
}
