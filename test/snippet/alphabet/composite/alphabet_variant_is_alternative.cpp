#include <bio/alphabet/composite/alphabet_variant.hpp>
#include <bio/alphabet/gap/gap.hpp>
#include <bio/alphabet/nucleotide/all.hpp>
#include <gtest/gtest.h>

int main()
{
    using variant_t = bio::alphabet_variant<bio::dna5, bio::gap>;

    static_assert(variant_t::is_alternative<bio::dna5>(), "dna5 is an alternative of variant_t");
    static_assert(!variant_t::is_alternative<bio::dna4>(), "dna4 is not an alternative of variant_t");
    static_assert(variant_t::is_alternative<bio::gap>(), "gap is an alternative of variant_t");
}
