#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/meta/char_operations/predicate.hpp>

int main()
{
    seqan3::is_in_alphabet<seqan3::dna4>('C');  // returns true

    auto constexpr my_check = seqan3::is_in_alphabet<seqan3::dna4>;
    my_check('U');  // returns false, because it comes out as 'T'
}
