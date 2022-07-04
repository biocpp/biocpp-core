#include <bio/meta/type_list/traits.hpp>

int main()
{
    using list_t = bio::meta::type_list<int, float, bool, double>;

    // Replace the second element with int.
    static_assert(std::same_as<bio::meta::type_list<int, int, bool, double>, bio::meta::list_traits::replace_at<int, 1, list_t>>);
}
