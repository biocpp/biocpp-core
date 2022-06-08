// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <gtest/gtest.h>

#include <string>

#include <bio/alphabet/all.hpp>
#include <bio/meta/char_operations/predicate.hpp>
#include <bio/meta/type_pack/detail/type_pack_algorithm.hpp>

using namespace std::literals;

template <char char_v>
struct foo : bio::detail::char_predicate_base<foo<char_v>>
{
    inline static const std::string msg{std::string{"foo_"} + std::string{char_v}};

    using base_t = bio::detail::char_predicate_base<foo<char_v>>;

    static auto constexpr data = [] () { typename base_t::data_t d{}; d[char_v] = true; return d; }();
};

template <char char_v>
inline constexpr foo<char_v> foo_v{};

struct bar
{
    template <typename alphabet_t>
    bool operator()([[maybe_unused]] alphabet_t c) const
    {
        return true;
    }
};

TEST(char_predicate, basic)
{
    foo<'a'> p{};
    EXPECT_TRUE(p('a'));
    EXPECT_FALSE(p('f'));
}

TEST(char_predicate, char_predicate_msg)
{
    EXPECT_EQ(foo<'o'>::msg, "foo_o"s);
}

TEST(char_predicate, concept)
{
    using bio::operator""_aa27;

    EXPECT_TRUE(bio::detail::char_predicate<decltype(bio::is_in_alphabet<bio::dna4>)>);
    EXPECT_TRUE(bio::detail::char_predicate<decltype(bio::is_char<bio::to_char('A'_aa27)>)>);
    EXPECT_TRUE((bio::detail::char_predicate<decltype(bio::is_in_interval<'a','z'>)>));
    EXPECT_TRUE(bio::detail::char_predicate<decltype(bio::is_space)>);
    EXPECT_TRUE(bio::detail::char_predicate<decltype(bio::is_blank)>);
    EXPECT_TRUE(bio::detail::char_predicate<decltype(bio::is_graph)>);
    EXPECT_TRUE(bio::detail::char_predicate<decltype(bio::is_alpha)>);
    EXPECT_TRUE(bio::detail::char_predicate<decltype(bio::is_digit)>);
    EXPECT_TRUE(bio::detail::char_predicate<decltype(bio::is_alnum)>);
    // TODO(rrahn): Direct call in concept caused ICE.
    auto val = ((!bio::is_space || bio::is_alpha) || bio::is_digit);
    EXPECT_TRUE(bio::detail::char_predicate<decltype(val)>);
    EXPECT_TRUE(bio::detail::char_predicate<foo<' '>>);
    EXPECT_FALSE(bio::detail::char_predicate<bar>);
    EXPECT_FALSE(bio::detail::char_predicate<int>);
}

TEST(char_predicate, char_predicate_combiner)
{
    using cond_t = bio::detail::char_predicate_combiner<foo<'a'>, foo<'A'>, foo<'0'>>;
    EXPECT_TRUE(cond_t{}('a'));
    EXPECT_TRUE(cond_t{}('A'));
    EXPECT_TRUE(cond_t{}('0'));
    EXPECT_FALSE(cond_t{}('z'));
    EXPECT_FALSE(cond_t{}('!'));
    EXPECT_FALSE(cond_t{}('1'));

    auto constexpr p = foo_v<'a'> || foo_v<'A'> || foo_v<'0'>;
    EXPECT_TRUE(p('a'));
    EXPECT_TRUE(p('A'));
    EXPECT_TRUE(p('0'));
    EXPECT_FALSE(p('z'));
    EXPECT_FALSE(p('!'));
    EXPECT_FALSE(p('1'));
}

TEST(char_predicate, char_predicate_combiner_msg)
{
    using or_t = bio::detail::char_predicate_combiner<foo<'a'>, foo<'A'>, foo<'0'>>;
    EXPECT_EQ(or_t::msg,   "(foo_a || foo_A || foo_0)"s);
}

TEST(char_predicate, is_not)
{
    using cond_t = bio::detail::char_predicate_negator<foo<'a'>>;
    EXPECT_FALSE(cond_t{}('a'));
    EXPECT_TRUE(cond_t{}('A'));
    EXPECT_TRUE(cond_t{}('0'));

    auto constexpr p = !foo_v<'a'>;
    EXPECT_FALSE(p('a'));
    EXPECT_TRUE(p('A'));
    EXPECT_TRUE(p('0'));
}

TEST(char_predicate, is_not_msg)
{
    using fn = decltype(!bio::is_alpha);
    EXPECT_EQ(fn::msg, "!(is_in_interval<'A', 'Z'> || is_in_interval<'a', 'z'>)"s);
}

TEST(char_predicate, is_in_interval)
{
    auto constexpr cond = bio::is_in_interval<'a', 'z'>;
    EXPECT_TRUE(cond('a'));
    EXPECT_TRUE(cond('k'));
    EXPECT_TRUE(cond('z'));
    EXPECT_FALSE(cond('A'));
    EXPECT_FALSE(cond('0'));
    EXPECT_FALSE(cond('!'));
}

TEST(char_predicate, is_in_interval_msg)
{
    EXPECT_EQ((bio::detail::is_in_interval_type<'a', 'z'>::msg), "is_in_interval<'a', 'z'>"s);
}

TEST(char_predicate, is_in_alphabet)
{
    {
        auto constexpr cond = bio::is_in_alphabet<bio::dna4>;
        EXPECT_TRUE(cond('a'));
        EXPECT_TRUE(cond('A'));
        EXPECT_TRUE(cond('c'));
        EXPECT_TRUE(cond('C'));
        EXPECT_TRUE(cond('g'));
        EXPECT_TRUE(cond('G'));
        EXPECT_TRUE(cond('t'));
        EXPECT_TRUE(cond('T'));
        EXPECT_FALSE(cond('N'));
        EXPECT_FALSE(cond('n'));
        EXPECT_FALSE(cond('!'));
        EXPECT_FALSE(cond('0'));
    }

    {
        auto constexpr cond = bio::is_in_alphabet<bio::aa27>;
        EXPECT_TRUE(cond('a'));
        EXPECT_TRUE(cond('A'));
        EXPECT_TRUE(cond('z'));
        EXPECT_TRUE(cond('Z'));
        EXPECT_TRUE(cond('*'));
        EXPECT_FALSE(cond('!'));
        EXPECT_FALSE(cond('0'));
    }
}

TEST(char_predicate, is_in_alphabet_msg)
{
    EXPECT_EQ((bio::detail::is_in_alphabet_type<bio::dna4>::msg), "is_in_alphabet<bio::dna4>"s);
}

TEST(char_predicate, is_char)
{
    using bio::operator""_aa27;
    {
        auto constexpr cond = bio::is_char<'A'>;
        EXPECT_TRUE(cond('A'));
        EXPECT_FALSE(cond('x'));
    }

    {
        auto constexpr cond = bio::is_char<bio::to_char('A'_aa27)>;
        EXPECT_TRUE(cond('A'));
        EXPECT_FALSE(cond('z'));
    }
}

TEST(char_predicate, is_char_msg)
{
    using bio::operator""_dna4;
    EXPECT_EQ((bio::is_char<bio::to_char('A'_dna4)>.msg), "is_char<'A'>"s);
    EXPECT_EQ((bio::is_char<'\t'>.msg), "is_char<'\t'>"s);
}

TEST(char_predicate, is_cntrl)
{
    EXPECT_TRUE(bio::is_cntrl('\0'));
    EXPECT_TRUE(bio::is_cntrl(static_cast<char>(31)));
    EXPECT_TRUE(bio::is_cntrl(static_cast<char>(127)));
    EXPECT_TRUE(bio::is_cntrl('\t'));
    EXPECT_FALSE(bio::is_cntrl('A'));
}

TEST(char_predicate, is_print)
{
    EXPECT_FALSE(bio::is_print('\0'));
    EXPECT_FALSE(bio::is_print(static_cast<char>(31)));
    EXPECT_FALSE(bio::is_print(static_cast<char>(127)));
    EXPECT_TRUE(bio::is_print(' '));
    EXPECT_TRUE(bio::is_print('A'));
    EXPECT_TRUE(bio::is_print('~'));
}

TEST(char_predicate, is_print_msg)
{
    EXPECT_EQ((bio::is_print.message()), "is_in_interval<' ', '~'>"s);
}


TEST(char_predicate, is_blank)
{
    EXPECT_TRUE(bio::is_blank(' '));
    EXPECT_TRUE(bio::is_blank('\t'));
    EXPECT_FALSE(bio::is_blank('A'));
    EXPECT_FALSE(bio::is_blank('\n'));
}

TEST(char_predicate, is_blank_msg)
{
    EXPECT_EQ((bio::is_blank.message()), "(is_char<'\t'> || is_char<' '>)"s);
}

TEST(char_predicate, is_space)
{
    EXPECT_TRUE(bio::is_space('\n'));
    EXPECT_TRUE(bio::is_space('\r'));
    EXPECT_TRUE(bio::is_space('\f'));
    EXPECT_TRUE(bio::is_space('\v'));
    EXPECT_TRUE(bio::is_space('\t'));
    EXPECT_TRUE(bio::is_space(' '));
    EXPECT_FALSE(bio::is_space('0'));
    EXPECT_FALSE(bio::is_space('\0'));
}

TEST(char_predicate, is_space_msg)
{
    EXPECT_EQ((bio::is_space.message()),
              "(is_in_interval<'\t', '\r'> || is_char<' '>)"s);
}

TEST(char_predicate, is_punct)
{
    EXPECT_TRUE(bio::is_punct('!'));
    EXPECT_TRUE(bio::is_punct('"'));
    EXPECT_TRUE(bio::is_punct('.'));
    EXPECT_TRUE(bio::is_punct('/'));
    EXPECT_TRUE(bio::is_punct(':'));
    EXPECT_TRUE(bio::is_punct('@'));
    EXPECT_TRUE(bio::is_punct('['));
    EXPECT_TRUE(bio::is_punct('`'));
    EXPECT_TRUE(bio::is_punct('{'));
    EXPECT_TRUE(bio::is_punct('~'));
    EXPECT_FALSE(bio::is_punct(' '));
    EXPECT_FALSE(bio::is_punct('0'));
    EXPECT_FALSE(bio::is_punct('\0'));
}

TEST(char_predicate, is_punct_msg)
{
    EXPECT_EQ((bio::is_punct.message()),
              "(((is_in_interval<'!', '/'> || is_in_interval<':', '@'>) || is_in_interval<'[', '`'>) || is_in_interval<'{', '~'>)"s);
}

TEST(char_predicate, is_alpha)
{
    EXPECT_FALSE(bio::is_alpha('\n'));
    EXPECT_FALSE(bio::is_alpha('\r'));
    EXPECT_FALSE(bio::is_alpha('\t'));
    EXPECT_FALSE(bio::is_alpha(' '));
    EXPECT_FALSE(bio::is_alpha('0'));
    EXPECT_TRUE(bio::is_alpha('a'));
    EXPECT_TRUE(bio::is_alpha('z'));
    EXPECT_TRUE(bio::is_alpha('Z'));
}

TEST(char_predicate, is_alpha_msg)
{
    EXPECT_EQ((bio::is_alpha.message()),
              "(is_in_interval<'A', 'Z'> || is_in_interval<'a', 'z'>)"s);
}

TEST(char_predicate, is_upper)
{
    EXPECT_FALSE(bio::is_upper('\n'));
    EXPECT_FALSE(bio::is_upper('\r'));
    EXPECT_FALSE(bio::is_upper('\t'));
    EXPECT_FALSE(bio::is_upper(' '));
    EXPECT_FALSE(bio::is_upper('0'));
    EXPECT_TRUE(bio::is_upper('A'));
    EXPECT_TRUE(bio::is_upper('Z'));
    EXPECT_FALSE(bio::is_upper('a'));
    EXPECT_FALSE(bio::is_upper('z'));
}

TEST(char_predicate, is_upper_msg)
{
    EXPECT_EQ((bio::is_upper.message()), "is_in_interval<'A', 'Z'>"s);
}

TEST(char_predicate, is_lower)
{
    EXPECT_FALSE(bio::is_lower('\n'));
    EXPECT_FALSE(bio::is_lower('\r'));
    EXPECT_FALSE(bio::is_lower('\t'));
    EXPECT_FALSE(bio::is_lower(' '));
    EXPECT_FALSE(bio::is_lower('0'));
    EXPECT_FALSE(bio::is_lower('A'));
    EXPECT_FALSE(bio::is_lower('Z'));
    EXPECT_TRUE(bio::is_lower('a'));
    EXPECT_TRUE(bio::is_lower('z'));
}

TEST(char_predicate, is_lower_msg)
{
    EXPECT_EQ((bio::is_lower.message()), "is_in_interval<'a', 'z'>"s);
}

TEST(char_predicate, is_digit)
{
    EXPECT_FALSE(bio::is_digit('\n'));
    EXPECT_FALSE(bio::is_digit('\r'));
    EXPECT_FALSE(bio::is_digit('\t'));
    EXPECT_FALSE(bio::is_digit(' '));
    EXPECT_TRUE(bio::is_digit('0'));
    EXPECT_TRUE(bio::is_digit('9'));
    EXPECT_FALSE(bio::is_digit('a'));
    EXPECT_FALSE(bio::is_digit('z'));
    EXPECT_FALSE(bio::is_digit('Z'));
}

TEST(char_predicate, is_digit_msg)
{
    EXPECT_EQ((bio::is_digit.message()), "is_in_interval<'0', '9'>"s);
}

TEST(char_predicate, is_xdigit)
{
    EXPECT_TRUE(bio::is_xdigit('0'));
    EXPECT_TRUE(bio::is_xdigit('9'));
    EXPECT_TRUE(bio::is_xdigit('a'));
    EXPECT_TRUE(bio::is_xdigit('f'));
    EXPECT_TRUE(bio::is_xdigit('A'));
    EXPECT_TRUE(bio::is_xdigit('F'));
    EXPECT_FALSE(bio::is_xdigit('g'));
    EXPECT_FALSE(bio::is_xdigit('z'));
    EXPECT_FALSE(bio::is_xdigit('G'));
    EXPECT_FALSE(bio::is_xdigit('Z'));
    EXPECT_FALSE(bio::is_xdigit('\n'));
    EXPECT_FALSE(bio::is_xdigit('\r'));
    EXPECT_FALSE(bio::is_xdigit('\t'));
    EXPECT_FALSE(bio::is_xdigit(' '));
}

TEST(char_predicate, is_xdigit_msg)
{
    EXPECT_EQ((bio::is_xdigit.message()), "((is_in_interval<'0', '9'> || is_in_interval<'A', 'F'>) || is_in_interval<'a', 'f'>)"s);
}

TEST(char_predicate, is_alnum)
{
    EXPECT_FALSE(bio::is_alnum('\n'));
    EXPECT_FALSE(bio::is_alnum('\r'));
    EXPECT_FALSE(bio::is_alnum('\t'));
    EXPECT_FALSE(bio::is_alnum(' '));
    EXPECT_TRUE(bio::is_alnum('0'));
    EXPECT_TRUE(bio::is_alnum('9'));
    EXPECT_TRUE(bio::is_alnum('a'));
    EXPECT_TRUE(bio::is_alnum('z'));
    EXPECT_TRUE(bio::is_alnum('Z'));
}

TEST(char_predicate, is_alnum_msg)
{
    EXPECT_EQ((bio::is_alnum.message()), "((is_in_interval<'0', '9'> || is_in_interval<'A', 'Z'>) || is_in_interval<'a', 'z'>)"s);
}

TEST(char_predicate, is_graph)
{
    EXPECT_FALSE(bio::is_graph('\n'));
    EXPECT_FALSE(bio::is_graph('\r'));
    EXPECT_FALSE(bio::is_graph('\t'));
    EXPECT_FALSE(bio::is_graph(' '));
    EXPECT_TRUE(bio::is_graph('0'));
    EXPECT_TRUE(bio::is_graph('9'));
    EXPECT_TRUE(bio::is_graph('a'));
    EXPECT_TRUE(bio::is_graph('z'));
    EXPECT_TRUE(bio::is_graph('Z'));
    EXPECT_TRUE(bio::is_graph('~'));
}

TEST(char_predicate, is_graph_msg)
{
    EXPECT_EQ((bio::is_graph.message()), "is_in_interval<'!', '~'>"s);
}

TEST(char_predicate, char_types)
{
    {  // is_char
        char c1 = '\t';
        EXPECT_TRUE(bio::is_char<'\t'>(c1));
        char16_t c2 = '\t';
        EXPECT_TRUE(bio::is_char<'\t'>(c2));
        char32_t c3 = '\t';
        EXPECT_TRUE(bio::is_char<'\t'>(c3));
    }

    {  // check value out of range.
        EXPECT_FALSE(bio::is_char<'\t'>(char16_t{256}));
    }

    {  // is_in_interval
        char c1 = 'n';
        EXPECT_TRUE((bio::is_in_interval<'a', 'z'>(c1)));
        char16_t c2 = 'n';
        EXPECT_TRUE((bio::is_in_interval<'a', 'z'>(c2)));
        char32_t c3 = 'n';
        EXPECT_TRUE((bio::is_in_interval<'a', 'z'>(c3)));
    }

    {  // check value out of range.
        EXPECT_FALSE((bio::is_in_interval<'a', 'z'>(char16_t{256})));
    }

    {  // is_in_alphabet
        char c1 = 'N';
        EXPECT_TRUE(bio::is_in_alphabet<bio::dna5>(c1));
        char16_t c2 = 'N';
        EXPECT_TRUE(bio::is_in_alphabet<bio::dna5>(c2));
        char32_t c3 = 'N';
        EXPECT_TRUE(bio::is_in_alphabet<bio::dna5>(c3));
    }

    {  // check value out of range
        EXPECT_FALSE(bio::is_in_alphabet<bio::dna5>(char16_t{256}));
    }
}

// see issue https://github.com/seqan/seqan3/issues/1972
TEST(char_predicate, issue1972)
{
    EXPECT_TRUE(bio::is_in_alphabet<bio::gapped<bio::rna5>>('A')); // valid bio::rna5 char
    EXPECT_TRUE(bio::is_in_alphabet<bio::gapped<bio::rna5>>('a')); // valid bio::rna5 char
    EXPECT_TRUE(bio::is_in_alphabet<bio::gapped<bio::rna5>>('-')); // valid bio::gap char
    EXPECT_FALSE(bio::is_in_alphabet<bio::gapped<bio::rna5>>('S')); // neither bio::rna5 nor bio::gap
}
