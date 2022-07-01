#include <bio/meta/type_list/traits.hpp>

int main()
{
    // Return the a type list of the pack without the first type.
    static_assert(std::same_as<bio::meta::type_list<float, bool, int>,
                               bio::meta::detail::pack_traits::drop_front<int, float, bool, int>>);
}
