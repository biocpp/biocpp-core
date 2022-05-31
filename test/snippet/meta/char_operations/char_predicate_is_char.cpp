#include <bio/meta/char_operations/predicate.hpp>

int main()
{
    bio::is_char<'C'>('C');  // returns true

    auto constexpr my_check = bio::is_char<'C'>;
    my_check('c');  // returns false, because case is different
}
