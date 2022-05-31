#include <bio/meta/char_operations/predicate.hpp>

int main()
{
    bio::is_in_interval<'A', 'G'>('C'); // returns true

    auto constexpr my_check = bio::is_in_interval<'A', 'G'>;
    my_check('H');  // returns false
}
