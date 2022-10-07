# Customisation {#biocpp_customisation}

BioC++ clearly documents which of its interfaces are designed to be used with user-provided types and how
this can be realised.

[TOC]

# Generic interfaces

Templates are a core part of generic programming in C++, because they allow instantiating the code with different
types or constants. BioC++'s API consists mostly of templates for exactly this reason, although we try to make this
less visible by having the template parameters deduced in many scenarios.

We strive to document the exact requirements for every template parameter in the API documentation; often these
are also enforced via C++ concepts. Wherever other types meet the given requirements, you can use these
types as arguments – independent of whether they are standard library types, your own types or types from another
library. See \ref core_concepts for more details on this.

Some requirements are more elaborate and depend on a specialisation or "overload" of another entity from the
BioC++ library. We call these "customisation points" (also known as "extension points").

# Customisation points

An example is the bio::alphabet::alphabet concept. One part of this concept's requirements are from
bio::alphabet::semialphabet which requires `to_rank()` to be available on an object of the type. `to_rank()`
is a "customisation point".

Instead of the bio::alphabet::semialphabet concept looking directly for `.to_rank()` member functions, it checks for
a valid definition of bio::alphabet::to_rank which in turn calls `tag_invoke()`.
This mechanism is widely employed in Modern C++ and may be standardised in the future
([P1895](https://wg21.link/P1895)).
There are (hidden) implementations of `tag_invoke()` that typically redirect to the respective member functions.

To customise one of our customisation points, follow the instructions in its API documentation. Typically, you just
have to add a `tag_invoke()` function for your type. This can happen as a friend or as a free function in your
type's namespace. The lookup happens via [argument dependent lookup](https://en.cppreference.com/w/cpp/language/adl).
The first argument of `tag_invoke()` is the "tag" used to select the implementation. In BioC++, these are named
exactly like the function that you otherwise call, except that they are in a `custom` subnamespace.
E.g. bio::alphabet::to_rank looks for `tag_invoke(bio::alphabet::custom::to_rank, YOUR_TYPE)`.

If you need to specialise the behaviour for a third party type, and you cannot open that type's namespace (or it doesn't have
one), you can also add your overload of `tag_invoke()` to our customisation namespace, e.g. `bio::alphabet::custom`.
Only do this, if there is no other solution.

In some cases, e.g. bio::alphabet::custom::enable_aminoacid, we also provide variable templates that you can specialise
directly.

\warning
**Never** add anything (types, functions, variables...) to any of our namespaces (except those named `custom`)
and never explicitly specialise one of our templates (except those in `custom` namespaces) or overload one of our functions.

The \link core_custom_alphabet HowTo on creating your own alphabet \endlink provides many examples of how to
satisfy the requirements of customisation point objects.

Since the customisation point objects are functors, *you can take their address* and use them in higher order functions.
