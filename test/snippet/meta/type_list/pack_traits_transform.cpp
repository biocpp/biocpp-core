#include <list>
#include <vector>

#include <bio/meta/type_list/traits.hpp>
#include <bio/ranges/type_traits.hpp>

int main()
{
    // Transform the types in the pack into reference types.
    static_assert(std::same_as<bio::meta::detail::pack_traits::transform<std::ranges::range_reference_t, std::vector<int>,
                                                                                              std::vector<float>,
                                                                                              std::list<bool>>,
                               bio::meta::type_list<int &, float &, bool &>>);
}
