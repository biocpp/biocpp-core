#include <bio/meta/type_list/traits.hpp>

int main()
{
    using list_t = bio::meta::type_list<int, float, bool, int>;

    static_assert(bio::meta::list_traits::contains<double, list_t> == false); // Is type double in type_list list_t?

    static_assert(bio::meta::list_traits::contains<float, list_t> == true);   // Is type float in type_list list_t?
}
