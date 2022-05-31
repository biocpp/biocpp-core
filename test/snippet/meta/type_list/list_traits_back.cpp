#include <bio/meta/type_list/traits.hpp>

int main()
{
    using list_t = bio::type_list<int, float, bool, int, float>;

    // Access the last value (float) with bio::list_traits::back
    static_assert(std::same_as<float, bio::list_traits::back<list_t>>);
}
