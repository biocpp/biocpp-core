# How to write your own alphabet {#core_custom_alphabet}

[TOC]

This HowTo documents how to write a custom alphabet that can be used with the algorithms and data structures in BioC++.

\tutorial_head{Moderate, 45 min, \ref core_concepts\, \ref core_alphabets, \ref alphabet}

# Motivation

In the [alphabet tutorial](\ref core_alphabets) you have learned that alphabets are a core component in BioC++
and represent the smallest unit of biological sequence data. We introduced the common alphabets for
nucleotide, amino acid and gap as well as structure and quality annotation.
However, in your BioC++ application you may want to implement a custom alphabet in order to work efficiently
with BioC++'s algorithms. To achieve this, your custom alphabet must meet certain requirements, which are defined
in [concepts](\ref core_concepts).

For detailed information on the alphabet concepts please read the \ref alphabet page.
In the following sections we demonstrate how to write an alphabet that models them.

A brief summary of the concepts used in this HowTo:
- bio::alphabet::semialphabet requires your type to have a numerical representation (rank), as well as
  an alphabet size and comparison operators.
- bio::alphabet::writable_semialphabet additionally requires being able to change the value of the object
  via the rank representation.
- bio::alphabet::alphabet requires that your type has a visual representation in addition to the numerical representation.
  Usually this is a character type like char.
- bio::alphabet::writable_alphabet additionally requires being able to change the value of the object
  via the char representation.

# Step by step: Create your own alphabet

In the alphabet tutorial we have calculated the GC content of a nucleotide sequence.
Guanine and Cytosine are complementary nucleobases,
which pair in a DNA molecule by building 3 hydrogen bonds. Adenine and Thymine pair with only 2 hydrogen bonds.
As a consequence, we denote Guanine and Cytosine as strong (S) and Adenine and Thymine as weak (W) nucleobases.
In this section we want to implement a bio::alphabet::alphabet that consists of the characters `S` and `W` to represent
strong and weak nucleobases.

Let's start with a simple struct that only holds the alphabet's numerical representation, namely the **rank** value.
It is not specified how the value of an alphabet is stored internally, however alphabets typically store the rank as a
member variable.
\snippet dna2_only_rank.cpp struct

\note
The type of the member variable is typically the smallest unsigned integer type that can hold the maximum rank.
In our case we have only two values so `bool` would be sufficient.
However `bool` is not actually smaller than `uint8_t` and does not always behave like an unsigned integral type
so we use `uint8_t` here. You only need a different type if your alphabet has an alphabet size >=255.

If you want BioC++'s algorithms to accept it as an alphabet, you need to make sure that your type
satisfies the requirements of the bio::alphabet::alphabet concept. A quick check can reveal that this is not the case:
\snippet dna2_only_rank.cpp alphabet_concept

## Prerequisites

A look at the documentation of bio::alphabet::alphabet will reveal that it is actually a refinement of other concepts,
more precisely bio::alphabet::semialphabet which in turn refines std::copy_constructible and std::totally_ordered.
Let's check those:
\snippet dna2_only_rank.cpp other_concepts

You should see that your type models only the std::copy_constructible concept.
Let's have a look at the documentation for std::totally_ordered. Can you guess what it describes?

It describes the requirements for types that are comparable via `==`, `!=`, `<`, `<=`, `>` and `>=`.

\assignment{Excercise}
Make all these operators available on `dna2`. HINT: Since C++20, you don't
actually need to implement every operator individually.
\endassignment
\solution
\snippet dna2_comparison_operators.cpp comparison
\endsolution

## Semialphabet

Let's move on to the more interesting concepts. bio::alphabet::semialphabet constitutes the *rank interface*
that we introduced in the [alphabet tutorial](\ref core_alphabets). Have a look at the API reference again.
Beyond the conceptional requirements, it also requires that bio::alphabet::size and bio::alphabet::to_rank can be
called on your alphabet.

Have a look at the respective API
reference and also the [documentation on customisation points](\ref biocpp_customisation).

You can add member functions for you type, and then make `tag_invoke()` wrappers,
but for simplicity, we will put the functionality directly into the `tag_invoke()` overloads here:

\snippet dna2_semialphabet.cpp semialphabet

As you can see from the `static_assert`, our dna2 alphabet now models bio::alphabet::semialphabet and
bio::alphabet::writable_semialphabet:
\snippet dna2_semialphabet.cpp writable_semialphabet_concept

You can also try out the customisation points directly (they appear like free functions). Here is an example:
\snippet dna2_semialphabet.cpp free_functions

## Alphabet

Now that you have a feeling for concepts, have a look at bio::alphabet::alphabet and bio::alphabet::writable_alphabet and make
your type also model these concepts.

\assignment{Excercise}
1. Implement the tag_invoke() function for "to_char" which returns the char 'S' or 'W' depending on the rank value.
2. Implement the tag_invoke() function for "assign_char".
3. There is a function object that tells us which characters are valid for a given alphabet: bio::alphabet::char_is_valid_for.
   By default, all characters are "valid" that are preserved when being assigned from and then be converted back.
   But in some cases you want to change the default behaviour, e.g. declaring lower-case letters to be valid, as well.
   <br>
   **Optional task:** Implement the tag_invoke() function for "char_is_valid"
   to also accept 's' and 'w' as valid characters.
\endassignment
\solution
\snippet dna2_alphabet.cpp writable_alphabet
\endsolution

At this point the bio::alphabet::alphabet concept should be modelled successfully and even bio::alphabet::writable_alphabet
is fine because we implemented `assign_char` and `char_is_valid`.
\snippet dna2_alphabet.cpp writable_alphabet_concept

## Shortcut: alphabet base template

Often it is not required to implement the entire class yourself, instead you can derive from bio::alphabet::alphabet_base.
The base class defines both members and tag_invoke() overloads for you––if you provide certain conversion tables. Read the documentation of
bio::alphabet::alphabet_base for details.
\snippet dna2_derive_from_base.cpp dna2

# Further examples
## Implementation as enum class

This is an example of a minimal custom alphabet that provides implementations for all necessary customisation
points.

As an enum class the values already have an order and therefore the class models std::totally_ordered
without defining the (in)equality and comparison operators. Opposed to the examples above, we use free functions
to implement the functionality (because we can't add friends to an enum).
\snippet test/unit/alphabet/custom_alphabet_test.cpp my_alph

## Adaptation of a third party type {#howto_write_an_alphabet_custom}

This example is similar to the previous one, but assuming that you cannot add anything to the namespace of
the type that you wish to adapt.
In that case, you need to add the tag_invoke() implementations to namespace
bio::alphabet::cpo so that they are found by our customisation points.

\snippet test/unit/alphabet/custom_alphabet3_test.cpp third_party_type

## Implementation of a non-default-constructible class

This is an example of a custom alphabet that is not default-constructible and that has a non-default overload for
bio::alphabet::char_is_valid_for.

Please note that for the overloads of bio::alphabet::size and bio::alphabet::char_is_valid_for our alphabet type has to
be wrapped into `std::type_identity<>` to be recognised by the customisation point objects, because the type does
not model std::is_nothrow_default_constructible after we have deleted the default constructor.

With the overload of bio::alphabet::char_is_valid_for, we allow assignment to the underlying rank type
from '1', 't' and 'T' for value 1 as well as from '0', 'f' and 'F' for value 0.

\snippet test/unit/alphabet/custom_alphabet2_test.cpp my_alph

\note
You should really make your alphabet types [no-throw-default-constructible](\ref std::is_nothrow_default_constructible)
if you can!

