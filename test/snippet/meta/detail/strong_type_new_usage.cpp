#include <vector>

#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/meta/detail/strong_type.hpp>
#include <ranges>

struct error : bio::detail::strong_type<unsigned, error>
{
    using bio::detail::strong_type<unsigned, error>::strong_type;
};

struct window_size : bio::detail::strong_type<unsigned, window_size>
{
    using bio::detail::strong_type<unsigned, window_size>::strong_type;
};

namespace bio::detail
{
template <std::ranges::forward_range fwd_rng_type>
    bool do_find(fwd_rng_type &, int const, int const) { return true; }
}  // namespace bio::detail

template <std::ranges::forward_range fwd_rng_type>
    bool search(fwd_rng_type & rng, window_size const w, error const e)
{
    // do something
    return bio::detail::do_find(rng, w.get(), e.get());
}

int main()
{
    using bio::operator""_dna4;

    std::vector<bio::dna4> my_range = "ACGTT"_dna4;
    // do something
    search(my_range, window_size{4}, error{2});
    // do something
    return 0;
}
