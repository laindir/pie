##NAME

pie - polynomial iteration encoding

##SYNOPSIS

##DESCRIPTION

A stream of binary data is representable as sequential values of the
dependent variable of a discrete pieciewise polynomial function over
a finite range, where the position is the independent variable.

For a given piece, the order of the polynomial is at most the number
of points minus one. In some cases, a polynomial with order
substantially less than this maximum can represent the same sequence
of points. In these cases, the polynomial representation need only
mention the number of iterations beyond the minimum (order plus one)
in order to recreate the original sequence.

If this number of iterations is greater than the overhead of the
metadata needed to process polynomials into original data streams,
this can represent a significant savings in the amount of data stored
or transmitted.

In order to save time in calculating the polynomial representation
and in recalculating the original byte sequence, we do not calculate
the coefficients to create a traditional "simplest terms" form.
Instead, we use forward differences to treat the polynomial as a
Newton series and record the differences directly. The original
values can then be calculated through a series of additions of these
differences--simply reversing the way in which they were calculated.

There is a possibility that a given stream may be representable by
multiple overlapping polynomials. The algorithm to be used for
optimal polynomial selection is an open question.

##EXAMPLES

To help explain the above, here are a series of 16 byte sequences
and their corresponding polynomial representation. Notice that all
calculations are carried out modulo 256.

1. constant

		00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00

		f(x) = 00

		0,,16

2. linear

		00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f
		01 01 01 01 01 01 01 01 01 01 01 01 01 01 01
		00 00 00 00 00 00 00 00 00 00 00 00 00 00

		f(x) = x

		2,00 01,14

3. quadratic

		00 01 04 09 10 19 24 31 40 51 64 79 90 a9 c4 e1
		01 03 05 07 09 0b 0d 0f 11 13 15 17 19 1b 1d
		02 02 02 02 02 02 02 02 02 02 02 02 02 02
		00 00 00 00 00 00 00 00 00 00 00 00 00

		f(x) = x * x

		3,00 01 02,13

4. cubic

		08 1b 40 7d d8 57 00 d9 e8 33 c0 95 b8 2f 00 31
		13 25 3d 5b 7f a9 d9 0f 4b 8d d5 23 77 d1 31
		12 18 1e 24 2a 30 36 3c 42 48 4e 54 5a 60
		06 06 06 06 06 06 06 06 06 06 06 06 06
		00 00 00 00 00 00 00 00 00 00 00 00

		f(x) = x * x * x + 6 * x * x + 12 * x + 8

		4,08 13 12 06,12

##AUTHOR

Written by Carl D Hamann (laindir).

##BUGS

Implementation incomplete

##COPYRIGHT

Copyright 2012 Carl D Hamann
Copying and distribution of this file, with or without modification,
are permitted in any medium without royalty provided the copyright
notice and this notice are preserved.  This file is offered as-is,
without any warranty.
