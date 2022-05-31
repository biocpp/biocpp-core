#include <string>

#include <bio/meta/debug_stream.hpp>
#include <bio/ranges/views/interleave.hpp>

int main()
{
    std::string u{"FOOBARBAXBAT"};
    std::string i{"in"};
    size_t s = 3;

    auto v = u | seqan3::views::interleave(s, i);

    seqan3::debug_stream << v << '\n'; // prints FOOinBARinBAXinBAT
}
