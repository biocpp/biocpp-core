#include <bio/meta/type_list/traits.hpp>

int main()
{
    using list_t = bio::type_list<int, float, bool, double>;

    // Replace the second element with int.
    static_assert(std::same_as<bio::type_list<int, int, bool, double>, bio::list_traits::replace_at<int, 1, list_t>>);
}
