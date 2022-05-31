#include <vector>

#include <bio/meta/debug_stream.hpp>
#include <bio/ranges/views/pairwise_combine.hpp>

int main()
{
    std::vector vec{'a', 'b', 'c', 'd'};
    for (auto res : vec | bio::views::pairwise_combine)
    {
        bio::debug_stream << res << "\n";
    }
}
