#include <bio/alphabet/composite/variant.hpp>
#include <bio/alphabet/gap/gap.hpp>
#include <bio/alphabet/nucleotide/all.hpp>
#include <gtest/gtest.h>

int main()
{
    using variant_t = bio::alphabet::variant<bio::alphabet::dna5, bio::alphabet::gap>;

    static_assert(variant_t::is_alternative<bio::alphabet::dna5>(), "dna5 is an alternative of variant_t");
    static_assert(!variant_t::is_alternative<bio::alphabet::dna4>(), "dna4 is not an alternative of variant_t");
    static_assert(variant_t::is_alternative<bio::alphabet::gap>(), "gap is an alternative of variant_t");
}
