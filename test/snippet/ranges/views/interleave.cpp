#include <string>

#include <bio/alphabet/fmt.hpp>
#include <bio/ranges/views/interleave.hpp>

int main()
{
    std::string u{"FOOBARBAXBAT"};
    std::string i{"in"};
    size_t s = 3;

    auto v = u | bio::views::interleave(s, i);

    fmt::print("{}\n", v); // prints FOOinBARinBAXinBAT
}
