#include <bio/meta/type_list/traits.hpp>

int main()
{
    using list_t = bio::meta::type_list<int, float, bool, int>;

    static_assert(bio::meta::list_traits::size<list_t> == 4);
}
