# pai

Proof of concept interpreter based on Flex and Bison (for a language somewhat
similar to Python).

Expression types:

* variables
* integers
* booleans
* read-only lists (`[1, 2, 3]`)
* read-only strings (`"mystring"`)
* operations with `+`, `-`, etc... (`myvar + 1 < 10`)
* list elements (i.e. `mylist[0]`)
* built-in functions (reading an integer: `read $ _`; getting the length of a
  string/list: `len $ mylist`)

Statement types:

* Expression-statement (prints the result of the expression: `1 + 2 + 3;`)
* Assignment (`mystring = "Hello, World!";`)
* If-statement (`if <expression> { <body...> }`)
* If-else-statement (`if <expression> { <body...> } else { <body...> }`)
* While-statement (`while <expression> { <body...> }`)

## Building

### Dependencies

* libfmt
* Clang version >= 11.0 (cannot be compiled with GCC because of some syntax
  limitations regarding non-trivial types in anonymous aggregates)
* flex
* bison

To build, run:

```
$ sh compile.sh
```

### Running

Test the interpreter on the [provided examples](./tests), such as:

```
$ ./pai tests/3.pai
```
