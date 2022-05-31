#include <bio/meta/type_list/traits.hpp>

int main()
{
    using list_t = bio::type_list<int, float, bool, int>;

    static_assert(bio::list_traits::size<list_t> == 4);
}
