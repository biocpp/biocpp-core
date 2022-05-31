#include <vector>

#include <bio/meta/type_traits/template_inspection.hpp>
#include <bio/meta/type_list/type_list.hpp>

int main()
{
    using my_type = std::vector<int>;

    if constexpr (bio::detail::is_type_specialisation_of_v<my_type, std::vector>) // std::vector has no <> !
    {
        // ...
    }
}
