#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/meta/char_operations/predicate.hpp>

int main()
{
    bio::is_in_alphabet<bio::dna4>('C');  // returns true

    auto constexpr my_check = bio::is_in_alphabet<bio::dna4>;
    my_check('U');  // returns false, because it comes out as 'T'
}
