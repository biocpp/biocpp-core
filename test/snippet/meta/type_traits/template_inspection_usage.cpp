#include <tuple>
#include <bio/meta/type_traits/template_inspection.hpp>
#include <bio/meta/type_list/type_list.hpp>

int main()
{
    using tl = bio::meta::type_list<int, char, double>;
    using t = bio::meta::transfer_template_args_onto_t<tl, std::tuple>;
    // t is std::tuple<int, char, double>
}
