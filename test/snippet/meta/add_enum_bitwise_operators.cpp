#include <bio/meta/add_enum_bitwise_operators.hpp>

enum class my_enum
{
    VAL1 = 1,
    VAL2 = 2,
    COMB = 3
};

template <>
constexpr bool bio::add_enum_bitwise_operators<my_enum> = true;

int main()
{
    using bio::operator|;

    my_enum e = my_enum::VAL1;
    my_enum e2 = e | my_enum::VAL2;

    // e2 == my_enum::COMB;
}
