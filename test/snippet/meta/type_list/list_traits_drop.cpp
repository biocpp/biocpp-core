#include <bio/meta/type_list/traits.hpp>

int main()
{
    using list_t = bio::type_list<int, float, bool, int>;

    // Drop the first two types in list_t.
    static_assert(std::same_as<bio::type_list<bool, int>, bio::list_traits::drop<2, list_t>>);
}
