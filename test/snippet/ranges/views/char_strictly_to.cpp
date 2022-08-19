#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/alphabet/fmt.hpp>
#include <bio/ranges/views/char_strictly_to.hpp>

int main()
{
    std::string_view str{"ACTTTGATAN"};
    try
    {
        fmt::print("{}", str | bio::ranges::views::char_strictly_to<bio::alphabet::dna4>); // ACTTTGATA
    }
    catch (bio::alphabet::invalid_char_assignment const &)
    {
        fmt::print("\n[ERROR] Invalid char!\n"); // Will throw on parsing 'N'
    }
}
