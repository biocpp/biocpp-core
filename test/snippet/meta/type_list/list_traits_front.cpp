#include <bio/meta/type_list/traits.hpp>

int main()
{
    using list_t = bio::meta::type_list<int, float, bool, int, float>;

    static_assert(std::same_as<int, bio::meta::list_traits::front<list_t>>); // Check if the first value is int.
}
