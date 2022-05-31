#include <bio/meta/type_list/traits.hpp>

int main()
{
    using list_t = bio::type_list<int, float, bool, int>;

    static_assert(std::same_as<bio::type_list<float, bool, int>, bio::list_traits::drop_front<list_t>>);
}
