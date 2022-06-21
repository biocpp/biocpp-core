#include <list>
#include <vector>

#include <bio/meta/type_list/traits.hpp>
#include <bio/meta/type_traits/range.hpp>

int main()
{
    // Transform the types in the pack into reference types.
    static_assert(std::same_as<bio::detail::pack_traits::transform<std::ranges::range_reference_t, std::vector<int>,
                                                                                              std::vector<float>,
                                                                                              std::list<bool>>,
                               bio::type_list<int &, float &, bool &>>);
}
