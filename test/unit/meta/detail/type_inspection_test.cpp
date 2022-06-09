// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <gtest/gtest.h>

#include <bio/meta/type_list/traits.hpp>
#include <bio/meta/type_list/type_list.hpp>
#include <bio/meta/detail/type_inspection.hpp>

// Some test namespace to check if namespace information are preserved within the naming.
namespace foo
{
template <typename ...type>
struct bar
{};
} // namespace foo

// Some types to test if type inspection works as expected.
// Note that the returned name might differ between compiler vendors and thus must be adapted accordingly
// in case this tests fails for those vendors.
using reflection_types = ::testing::Types<char, char16_t const, char32_t &, short *, double const * const,
                                          foo::bar<char> const &, foo::bar<foo::bar<char, double>>>;

// Helper type list to use some traits functions on type lists.
using as_type_list_t = bio::detail::transfer_template_args_onto_t<reflection_types, bio::type_list>;

template <typename param_type>
class type_inspection : public ::testing::Test
{
    // The corresponding list of names that should be generated. Must have the same order as `reflection_types`.
    inline static const std::vector names{"char", "char16_t const", "char32_t &", "short*", "double const* const",
                                          "foo::bar<char> const &", "foo::bar<foo::bar<char, double> >"};

public:
    // Returns the name of the type according to the list of names defined above.
    std::string expected_name()
    {
        return names[bio::list_traits::find<param_type, as_type_list_t>];
    }
};

// Register test.
TYPED_TEST_SUITE(type_inspection, reflection_types, );

TYPED_TEST(type_inspection, type_name_as_string)
{
    EXPECT_EQ(bio::detail::type_name_as_string<TypeParam>, this->expected_name());
}
