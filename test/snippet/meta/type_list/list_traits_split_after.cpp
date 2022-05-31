#include <bio/meta/type_list/traits.hpp>

int main()
{
    using list_t = bio::type_list<int, float, bool, double, char, int>;
    using split_t = bio::list_traits::split_after<3, list_t>;

    // Use ::first_type and ::second_type to access the type lists after being split.
    static_assert(std::same_as<bio::type_list<int, float, bool>,
                               split_t::first_type>);
    static_assert(std::same_as<bio::type_list<double, char, int>,
                               split_t::second_type>);
}
