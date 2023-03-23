#include <array>
#include <variant>

#include <bio/meta/tag/vtag.hpp>
#include <bio/ranges/container/dictionary.hpp>

using namespace std::string_literals;
using namespace std::string_view_literals;
using namespace bio::meta::literals;

/* A type to hold an optional field in a SAM record.
 *
 * See https://samtools.github.io/hts-specs/SAMtags.pdf for how this is used.
 *
 * These optional fields can have data of different types, e.g. string and int (other types
 * are ignored for this simple example). The way to store this in C++ is a std::variant.
 *
 * Since the relationship between the identifier tag and the type is usually known,
 * we can provide a string-based get<>() interface.
 */
struct sam_optional_field : std::variant<std::string, int32_t>
{
    /* Declare the get<"foo">(obj) interface; overloads for cv-qualified versions omitted here. */
    template <bio::ranges::small_string tag>
    friend decltype(auto) get(sam_optional_field const & me)
    {
        static_assert(contains(as_strings, tag) || contains(as_ints, tag),
                      "Don't know how to choose type for given id.");

        if constexpr (contains(as_strings, tag))
        {
            return std::get<std::string>(me);
        }
        else /* is int */
        {
            return std::get<int32_t>(me);
        }
    }

private:
    /* Define which tags result in which types; only four example tags defined here. */
    static constexpr auto as_strings = std::array{"CO"sv, "OA"sv};
    static constexpr auto as_ints    = std::array{"AS"sv, "NM"sv};

    /* In C++23, you can use std::ranges::contains() instead. */
    static constexpr bool contains(auto const & arr, std::string_view const val)
    {
        return std::ranges::find(arr, val) != arr.end();
    }
};

int main()
{
    bio::ranges::dictionary<std::string, sam_optional_field, true> sam_optional_fields;

    /* You can easily insert elements of different types, but you need to make sure they match the predefined IDs! */
    sam_optional_fields.emplace_back("CO", "This is a comment.");
    sam_optional_fields.emplace_back("AS", 42);                    // Alignment score 42
    sam_optional_fields.emplace_back("NM", 23);                    // Edit distance 23
    sam_optional_fields.emplace_back("OA", "23M3I11M");            // Original cigar

    // std::string s = sam_optional_fields["CO"];   // ["CO"] returns sam_optional_field
    std::string s = sam_optional_fields["CO"_vtag]; // ["CO"_vtag] returns std::string (type "inside variant")
    fmt::print("{}\n", s);                          // prints "This is a comment."

    // int32_t i = sam_optional_fields["AS"];       // ["AS"] returns sam_optional_field
    int32_t i = sam_optional_fields["AS"_vtag];     // ["AS"_vtag] returns int (type "inside variant")
    fmt::print("{}\n", i);                          // prints "42"
}
