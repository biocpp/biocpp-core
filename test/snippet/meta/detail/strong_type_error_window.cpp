#include <bio/meta/detail/strong_type.hpp>

struct error : bio::detail::strong_type<unsigned, error>
{
    using bio::detail::strong_type<unsigned, error>::strong_type;
};

struct window_size : bio::detail::strong_type<unsigned, window_size>
{
    using bio::detail::strong_type<unsigned, window_size>::strong_type;
};
