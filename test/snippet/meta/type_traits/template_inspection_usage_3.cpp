#include <vector>

#include <bio/meta/type_traits/template_inspection.hpp>

int main()
{
    using my_type = std::vector<int>;

    if constexpr (bio::detail::template_specialisation_of<my_type, std::vector>) // Note: std::vector has no <> !
    {
        // ...
    }
}
