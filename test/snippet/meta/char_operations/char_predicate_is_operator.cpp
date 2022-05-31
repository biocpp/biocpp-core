#include <bio/meta/char_operations/predicate.hpp>

int main()
{
    char chr{'1'};
    auto constexpr my_cond = bio::is_char<'%'> || bio::is_digit;
    bool is_percent = my_cond(chr); // is_percent == true
}
