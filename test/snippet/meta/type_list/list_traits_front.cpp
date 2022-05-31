#include <bio/meta/type_list/traits.hpp>

int main()
{
    using list_t = bio::type_list<int, float, bool, int, float>;

    static_assert(std::same_as<int, bio::list_traits::front<list_t>>); // Check if the first value is int.
}
