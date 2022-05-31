#include <list>
#include <vector>

#include <bio/meta/type_list/traits.hpp>
#include <bio/meta/type_traits/range.hpp>

int main()
{
    using list_t = bio::type_list<std::vector<int>, std::vector<float>, std::list<bool>>;

    // Transform the types into reference types.
    static_assert(std::same_as<bio::list_traits::transform<std::ranges::range_reference_t, list_t>,
                               bio::type_list<int &, float &, bool &>>);
}
