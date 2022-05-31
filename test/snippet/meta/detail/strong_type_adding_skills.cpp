#include <bio/meta/detail/strong_type.hpp>

using bio::operator|;

struct error : bio::detail::strong_type<unsigned, error, bio::detail::strong_type_skill::decrement |
                                                            bio::detail::strong_type_skill::increment>
{
    using bio::detail::strong_type<unsigned, error, bio::detail::strong_type_skill::decrement |
                                                       bio::detail::strong_type_skill::increment>::strong_type;
};

int main()
{
    error e{4};
    --e;
    ++e;
}
