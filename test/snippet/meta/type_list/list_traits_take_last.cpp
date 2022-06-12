#include <bio/meta/type_list/traits.hpp>

int main()
{
    using list_t = bio::type_list<int, float, bool, int>;

    // Take the last two types in list_t.
    static_assert(std::same_as<bio::type_list<bool, int>, bio::list_traits::take_last<2, list_t>>);
}