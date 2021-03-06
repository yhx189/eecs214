# CS 214 Homework 1: Huffman Coding

  - Due: Friday, October 24 at 5:00 PM<sup>[‡](#fine-print)</sup>

  - May optionally be completed with one partner of your choosing

## Summary

For this homework, you will write two programs:

  - `huff` compresses text files using Huffman coding.

  - `puff` decompresses Huffman-coded files produced by `huff`, giving
    back the original file.

The filenames to read from and write to are passed to each program as
command-line arguments. For example, suppose you have a text file
`hamlet.txt` that you'd like to compress. You can compress it to a file
`hamlet.txt.huff` using your `encode` program:

```sh
    % ./huff hamlet.txt hamlet.txt.huff
```

You can decompress it using your `decode` program:

```sh
    % ./puff hamlet.txt.huff hamlet.txt.out
```

If you’ve done your job correctly, the decompressed file will match the
original. On UNIX (including Linux and Mac OS X) you can compare the two
to make sure they match using `diff`:

```sh
    % diff hamlet.txt hamlet.txt.out
```

When two files match `diff` prints nothing.

### Goals

The primary goal of this assignment is to get you started at programming
data structures. You will also become familiar with a specific data
structure, Huffman trees, and will think about how data structures can
be serialized—that is, converted to a sequence of bits that can be
written to a file.

## Getting started

I've posted starter code, including bitwise IO libraries, for the five
languages we chose:

  - [C](https://github.com/tov/eecs214/tree/master/lib/bit-io/c)
  - [C++](https://github.com/tov/eecs214/tree/master/lib/bit-io/cpp)
  - [Java](https://github.com/tov/eecs214/tree/master/lib/bit-io/java)
  - [Python](https://github.com/tov/eecs214/tree/master/lib/bit-io/python)
  - [Ruby](https://github.com/tov/eecs214/tree/master/lib/bit-io/ruby)

You only need one, but the best way to get this code is to clone [the
whole course Git repository](https://github.com/tov/eecs214), which
includes all five languages in directory
[`lib/bit-io`](https://github.com/tov/eecs214/tree/master/lib/bit-io).
Or you can simply download the whole course repository as a [ZIP
file](https://github.com/tov/eecs214/archive/master.zip)

The starter code for each language includes a version of `encode`
that---rather than Huffman coding---compresses an ASCII text file to 7/8
of its original size using a simple block code. The next section
discusses details of how that works, but first we will consider how to
build, if necessary, and run the code in each language:

### C

TODO.

### C++

TODO.

### Java

TODO.

### Python

TODO.

### Ruby

TODO.

## Warmup: ASCII block code

Most computers today have 8-bit bytes, but the
[ASCII](https://en.wikipedia.org/wiki/ASCII) text encoding requires only
7 bits per character. ASCII text files use one 8-bit byte for each 7-bit
character, leaving the high-order bit unused. (Eight-bit text encodings
such as [Latin-1](https://en.wikipedia.org/wiki/ISO/IEC_8859-1) use the
high bit to double the number of representable characters for writing a
variety of Latin-alphabet–based languages other than English.) Thus,
each ASCII character is 7 bits of information represented in an 8-bit
block code.

Given ASCII text (rather than, say, Latin-1), we can avoid storing each
byte's high bit using a 7-bit code word, and pack them into bytes. For
example, the string “ABCDEFGH” is represented in ASCII using 8 bytes,
which we can write out in binary like so:

```
01000001 01000010 01000011 01000100 01000101 01000110 01000111 01001000
```

Because ASCII does not use the high bit, the first bit in each octet is
0. Since it will never be 1, we don't need to represent it:

```
1000001 1000010 1000011 1000100 1000101 1000110 1000111 1001000
```

We can then “pack” the eight 7-bit code words above by regrouping the
bits back into octets:

```
10000011 00001010 00011100 01001000 10110001 10100011 11001000
```

The program `encode` converts from the ordinary 8-bit format to the
packed 7-bit format, and `decode` converts back. However, there are two
caveats:

  - This encoding means that if your file has any non-ASCII characters,
    they won’t survive the encoding.

  - For some file sizes, the round-tripped (encoded and then decoded)
    file has an additional, final byte that wasn't in the original file.

Consider the following questions, and feel free to discuss on Piazza:

  - What is the problem with `encode` and `decode`? When is the extra
    byte added, and why?

  - Can you think of a way to fix the problem? Assume that you are
    allowed to change the file format used by `encode` and `decode` in a
    way that adds a *constant* number of bits to the encoded file; (That
    is, the number of bits added should not depend on the original file
    size.)

    Can you think of a fix that works even if the file is
    streaming? That is, suppose that you cannot see the whole file at
    once, but rather must begin producing output before the input ends.

## The Huffman coder and decoder

Your main task is to write the two programs `huff` and `puff` as
described in the introduction. In particular:

  - `huff` reads an unencoded input file, and writes the Huffman-coded
    results an output file.

  - `puff` reads a Huffman-coded input file, and writes the decoded
    results an output file.

When `huff` reads an input file, it reads it as a sequence of bytes,
that is, values ranging from 0 to 255. In coding terms, the input
language’s alphabet has 256 different symbols. `huff` counts how many
times each of these 256 symbols occurs in the file and uses these counts
as weights to construct a Huffman tree; thus, the leaves of the tree
will be byte values. It then uses the Huffman tree to write an encoded
version of the input file to its output file.

The job of `puff` is to read a `huff`-encoded file and produce the
original file. However, there are two reasons this task is harder than
it may sound:

1.  The number of bits that the encoding requires may not be a multiple
    of 8, in which case the remaining bits in the file must be padded
    somehow. How can the decoder tell padding bits from data bits?

2.  The decoder needs some way to recover the same Huffman tree that
    `huff` used to encode it. Since different files have different
    letter frequencies, `huff` needs to somehow transmit the tree to
    `puff` in a file header—that is, metadata written at the beginning
    of a file before the main data starts. Designing the file format is
    up to you. Here are two ideas:

      - Serialize (that is, convert to a sequence of bits) the tree
        using a pre-order traversal, as follows:

          - At every leaf, output a 0 bit followed by the 8 bits
            representing the input symbol on that leaf.

          - At every branch, output a 1 bit, followed by left branch and
            then the right branch.

      - Instead of storing the tree, store the frequency table used to
        build the tree. Provided your Huffman-tree algorithm is
        deterministic, you should be able to reconstruct the same tree
        in `puff` as was used in `huff`.

Feel free to discuss these questions on Piazza. Note that because you
are designing your own file format, your file format will not
necessarily match that of other students.

## Language-dependent details

### C

TODO.

### C++

TODO.

### Java

TODO.

### Python

TODO.

### Ruby

TODO.

## Deliverables

Your deliverables are:

  - The source code of the `huff` and `puff` programs as described
    above.

  - A plain text file `README.md` (which can use
    [Markdown](https://help.github.com/articles/github-flavored-markdown/)
    formatting if you like) in which you describe the design of your
    program. In particular please discuss, briefly:

      - how your file format solves the file length and tree
        transmission problems described above;

      - how you tested your programs to ensure correctness; and

      - anything that surprised you while doing this assignment.

    When discussing your code, please provide file and line number
    references.

You will be graded on:

  - the functional correctness of your code, as determined via automated
    testing,

  - whether your code implements the specified data structure (in this
    case a Huffman tree), as determined by reading it, and

  - the quality of your `README.md` answers.

## How to submit

TBD

## Fine print

‡ Free weekend extensions until noon on Monday
