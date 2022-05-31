#include <bio/meta/type_list/traits.hpp>

int main()
{
    // Return the a type list of the pack without the first type.
    static_assert(std::same_as<bio::type_list<float, bool, int>,
                               bio::pack_traits::drop_front<int, float, bool, int>>);
}
