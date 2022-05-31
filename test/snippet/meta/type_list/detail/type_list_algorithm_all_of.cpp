#include <type_traits>

#include <bio/meta/type_list/detail/type_list_algorithm.hpp>
#include <bio/meta/type_list/type_list.hpp>

namespace incomplete
{

struct type;

} // namespace incomplete

// With c++20 you could also write it like this
// auto fn = []<typename type>(std::type_identity<type>)
// {
// ...
// };
auto fn = [](auto id)
{
    // id is of type std::type_identity<type>
    using id_t = decltype(id);
    using type = typename id_t::type;

    static_assert(std::is_same_v<id_t, std::type_identity<type>>, "id is of type std::type_identity<type>");

    if constexpr(std::is_same_v<type, bool>)
        return true;
    else if constexpr(std::is_same_v<type, int>)
        return true;
    else if constexpr(std::is_same_v<type, float>)
        return true;
    else if constexpr(std::is_same_v<type, incomplete::type>)
        return false;
};

static_assert(bio::detail::all_of<bio::type_list<int, float, bool>>(fn)); // evalates to true
static_assert(!bio::detail::all_of<bio::type_list<int, float, bool, incomplete::type>>(fn)); // evalates to false
