#include <bio/meta/type_list/traits.hpp>

int main()
{
    using list_t = bio::meta::type_list<int, float, bool, int>;

    // Count the number of type int in list_t.
    static_assert(bio::meta::list_traits::count<int, list_t> == 2);
}
