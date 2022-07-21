// This example illustrates how we can reduce the usage of templates (or the amount of different instantiations) via
// type erasure. Having only one function generated for `algorithm()` is the only benefit of using `semialphabet_any`
// here. Of course this only makes sense for your application if the part of the program that is agnostic of the
// character representation (your equivalent of `algorithm()`) is substantially larger than the specific parts – and
// if compile-time and/or size of the exectuble are a concern.

#include <iostream>

#include <bio/alphabet/aminoacid/all.hpp>
#include <bio/alphabet/composite/semialphabet_any.hpp>
#include <bio/alphabet/fmt.hpp>
#include <bio/ranges/views/convert.hpp>
#include <bio/ranges/views/to.hpp>

using namespace bio::alphabet::literals;

// Print is a template and gets instantiated two times because the behaviour is different for both types
template <typename rng_t>
void print(rng_t && r)
{
    fmt::print("{}\n", r);
}

// Algorithm is not a template, only one instance is generated by the compiler
// Type information is encoded via a run-time parameter
void algorithm(std::vector<bio::alphabet::semialphabet_any<10> > & r, bool is_murphy)
{
    // Algorithm example that replaces rank 0 with rank 1
    for (auto & v : r)
        if (bio::alphabet::to_rank(v) == 0)
            bio::alphabet::assign_rank_to(1, v);

    // Here we reify the type for printing
    if (is_murphy)
        print(r | bio::views::convert<bio::alphabet::aa10murphy>);
    else
        print(r | bio::views::convert<bio::alphabet::aa10li>);
}

// Two instances of algo_pre exist
// They type erase the different arguments to the same type and encode the type information as a run-time parameter
void algo_pre(bio::alphabet::aa10li_vector const & v)
{
    std::vector<bio::alphabet::semialphabet_any<10> > tmp = v | bio::views::convert<bio::alphabet::semialphabet_any<10>>
                                                       | bio::views::to<std::vector>();
    algorithm(tmp, false);
}

void algo_pre(bio::alphabet::aa10murphy_vector const & v)
{
    std::vector<bio::alphabet::semialphabet_any<10> > tmp = v | bio::views::convert<bio::alphabet::semialphabet_any<10>>
                                                       | bio::views::to<std::vector>();
    algorithm(tmp, true);
}

int main()
{
    bio::alphabet::aa10li_vector v1{"AVRSTXOUB"_aa10li};
    algo_pre(v1); // BIKBBBKCB

    bio::alphabet::aa10murphy_vector v2{"AVRSTXOUB"_aa10murphy};
    algo_pre(v2); // BIKSSSKCB
}
