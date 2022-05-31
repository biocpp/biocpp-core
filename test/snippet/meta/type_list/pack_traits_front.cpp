#include <bio/meta/type_list/traits.hpp>

int main()
{
    // Check if the first value is int.
    static_assert(std::same_as<int, bio::pack_traits::front<int, float, bool, int, float>>);
}
