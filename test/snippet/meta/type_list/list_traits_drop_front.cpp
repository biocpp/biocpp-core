#include <bio/meta/type_list/traits.hpp>

int main()
{
    using list_t = bio::meta::type_list<int, float, bool, int>;

    static_assert(std::same_as<bio::meta::type_list<float, bool, int>, bio::meta::list_traits::drop_front<list_t>>);
}
