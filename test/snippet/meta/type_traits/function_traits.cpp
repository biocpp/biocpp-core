#include <cassert>
#include <functional>
#include <string>

#include <bio/meta/type_traits/function.hpp>
#include <concepts>

std::function my_caller = [] (size_t position, std::string & sequence)
{
    assert(position < sequence.size());
    return sequence[position];
};

using my_function_t = decltype(my_caller);

static_assert(std::same_as<bio::function_traits<my_function_t>::result_type, char>);
static_assert(bio::function_traits<my_function_t>::argument_count == 2);
static_assert(std::same_as<bio::function_traits<my_function_t>::argument_type_at<0>, size_t>);
static_assert(std::same_as<bio::function_traits<my_function_t>::argument_type_at<1>, std::string &>);
