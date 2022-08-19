#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/alphabet/fmt.hpp>
#include <bio/ranges/views/validate_char_for.hpp>

int main()
{
    std::string_view str{"ACTTTGATAN"};
    try
    {
        fmt::print("{}", str | bio::ranges::views::validate_char_for<bio::alphabet::dna4>);
        //prints: ACTTTGATA
    }
    // Will throw on parsing 'N'
    catch (bio::alphabet::invalid_char_assignment const &)
    {
        fmt::print("\n[ERROR] Invalid char!\n");
    }
}
