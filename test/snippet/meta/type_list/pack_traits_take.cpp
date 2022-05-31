#include <seqan3/core/type_list/traits.hpp>

int main()
{

    // Take the first two types in the pack.
    static_assert(std::same_as<seqan3::type_list<int, float>, seqan3::pack_traits::take<2, int, float, bool, double>>);
}
