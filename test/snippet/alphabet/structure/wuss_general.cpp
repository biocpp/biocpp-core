#include <bio/alphabet/structure/wuss.hpp>
#include <bio/meta/debug_stream.hpp>

int main()
{
    using bio::operator""_wuss51;

    // create vector
    std::vector<bio::wuss51> vec{'.'_wuss51, '>'_wuss51, '>'_wuss51};
    // modify and print
    vec[1] = '<'_wuss51;
    for (bio::wuss51 chr : vec)
        bio::debug_stream << bio::to_char(chr);  // .<>
    bio::debug_stream << "\n";
}
