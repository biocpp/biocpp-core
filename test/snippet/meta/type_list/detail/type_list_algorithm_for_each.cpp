#include <string>

#include <bio/alphabet/fmt.hpp>
#include <bio/meta/type_list/function.hpp>
#include <bio/meta/type_list/type_list.hpp>

namespace incomplete
{

struct type;

} // namespace incomplete

int main()
{
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
            fmt::print("{}", "bool");
        else if constexpr(std::is_same_v<type, int>)
            fmt::print("{}", "int");
        else if constexpr(std::is_same_v<type, float>)
            fmt::print("{}", "float");
        else if constexpr(std::is_same_v<type, incomplete::type>)
            fmt::print("{}", "incomplete::type");

        fmt::print(", ");
    };

    // prints each type name, i.e. "int, float, bool, incomplete::type, \n"
    using types = bio::type_list<int, float, bool, incomplete::type>;
    bio::detail::for_each<types>(fn);
    fmt::print("\n");

    // is the same as explicitly writing
    fn(std::type_identity<int>{});
    fn(std::type_identity<float>{});
    fn(std::type_identity<bool>{});
    fn(std::type_identity<incomplete::type>{});
    fmt::print("\n");
    return 0;
}
