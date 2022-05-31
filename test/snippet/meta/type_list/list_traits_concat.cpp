#include <bio/meta/type_list/traits.hpp>

int main()
{
    using list_t = bio::type_list<int, float, bool>;
    using list_t2 = bio::type_list<double, char, int>;
    using list_t3 = bio::type_list<int, int>;

    static_assert(std::same_as<bio::list_traits::concat<list_t, list_t2, list_t3>,
                               bio::type_list<int, float, bool, double, char, int, int, int>>);
}
