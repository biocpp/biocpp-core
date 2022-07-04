#include <bio/meta/type_list/traits.hpp>

int main()
{
    using list_t = bio::meta::type_list<int, float, bool, double>;

    // Look at the 2nd element.
    static_assert(std::same_as<float, bio::meta::list_traits::at<1, list_t>>);
    // Look at the last element.
    static_assert(std::same_as<double, bio::meta::list_traits::at<-1, list_t>>);
}
