#include <bio/meta/type_list/traits.hpp>

int main()
{
    using list_t = bio::type_list<int, float, bool>;

    // Double is not in list_t so find returns -1. However, bool is in the type list so find will return 2.
    static_assert(bio::list_traits::find<double, list_t> == -1);
    static_assert(bio::list_traits::find<bool, list_t> == 2);
}
