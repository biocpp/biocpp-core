#include <deque>
#include <forward_list>
#include <list>
#include <vector>

#include <bio/ranges/views/to.hpp>

int main()
{
    auto lst = std::views::iota(1, 10); // some range over the numbers 1-10

    // convert range to vector using pipe syntax
    auto vec0 = lst | bio::views::to<std::vector<int>>();
    static_assert(std::same_as<decltype(vec0), std::vector<int>>);

    // convert range to vector but auto deducing the element type
    auto vec1 = lst | bio::views::to<std::vector>();
    static_assert(std::same_as<decltype(vec1), std::vector<int>>);

    // convert range to vector using function call syntax
    auto vec2 = bio::views::to<std::vector<int>>(lst);
    static_assert(std::same_as<decltype(vec2), std::vector<int>>);

    // using function call syntax and auto deducing element type
    auto vec3 = bio::views::to<std::vector>(lst);
    static_assert(std::same_as<decltype(vec3), std::vector<int>>);

    // convert nested ranges into nested containers
    auto nested_lst = std::list<std::forward_list<int>>{{1, 2, 3}, {4, 5, 6, 7}};
    auto vec4 = nested_lst | bio::views::to<std::vector<std::vector<int>>>();
    static_assert(std::same_as<decltype(vec4), std::vector<std::vector<int>>>);

    // different supported container types
    auto vec5 = lst | bio::views::to<std::list>();
    static_assert(std::same_as<decltype(vec5), std::list<int>>);

    auto vec6 = lst | bio::views::to<std::deque>();
    static_assert(std::same_as<decltype(vec6), std::deque<int>>);
}
