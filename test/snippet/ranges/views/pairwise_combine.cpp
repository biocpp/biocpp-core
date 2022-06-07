#include <vector>

#include <bio/alphabet/fmt.hpp>
#include <bio/ranges/views/pairwise_combine.hpp>

int main()
{
    std::vector vec{'a', 'b', 'c', 'd'};
    for (auto res : vec | bio::views::pairwise_combine)
    {
        fmt::print("{}\n", res);
    }
}
